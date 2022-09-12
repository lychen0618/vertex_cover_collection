#include <grpc/grpc.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include <iostream>
#include <thread>

#include "advanced_adc_enum.h"
#include "protos/vertex_cover.grpc.pb.h"
#include "random_enum_auto_inc.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using vcc::GenRequest;
using vcc::GenResponse;
using vcc::ValueIntList;
using vcc::VertexCoverGenerator;
using vcc::VertexCoverList;

using vcc::AdvancedADCEnum;
using vcc::HyperGraph;
using vcc::IntSet;
using vcc::OutputQueue;
using vcc::RandomEnumAutoInc;

class VertexCoverGeneratorServiceImpl final
    : public VertexCoverGenerator::Service {
    void Response(ServerWriter<GenResponse>* writer,
                  std::shared_ptr<OutputQueue> output_queue,
                  bool& done) {
        while (!done) {
            if (output_queue->Size() != 0) {
                IntSet mvc = std::move(output_queue->Pop());
                GenResponse response;
                ValueIntList* vc =
                    response.mutable_vertex_cover_list()->add_vertex_cover();
                for (size_t i = 0; i < mvc.Size(); ++i) {
                    vc->add_value(mvc.Get(i));
                }
                writer->Write(response);
            } else
                std::this_thread::yield();
        }
    }

    Status Generate(ServerContext* context,
                    const GenRequest* request,
                    ServerWriter<GenResponse>* writer) override {
        std::cout << "Received one request\n";
        // Construct hypergraph
        std::vector<IntSet> edge_list;
        const auto& raw_hypergraph = request->hyper_graph();
        for (int i = 0; i < raw_hypergraph.edge_size(); ++i) {
            IntSet edge;
            for (int j = 0; j < raw_hypergraph.edge(i).value_size(); ++j) {
                edge.Add(raw_hypergraph.edge(i).value(j));
            }
            edge_list.emplace_back(std::move(edge));
        }
        std::shared_ptr<HyperGraph> hyper_graph(new HyperGraph(edge_list));
        std::shared_ptr<OutputQueue> output_queue(new OutputQueue(1000));
        bool done = false;
        std::thread child(&VertexCoverGeneratorServiceImpl::Response, this,
                          writer, output_queue, std::ref(done));
        // Call specific algorithms according to request type
        if (request->request_type() == vcc::RequestType::DIVERSITY) {
            int k = static_cast<int>(request->parameter());
            RandomEnumAutoInc random_enum_auto_inc(hyper_graph, output_queue,
                                                   k);
            random_enum_auto_inc.SetMethod(vcc::MMCS::Method::MIN);
            random_enum_auto_inc.SetToLog(false);
            random_enum_auto_inc.SetToQueue(true);
            random_enum_auto_inc.RunDiverse();
        } else if (request->request_type() == vcc::RequestType::APPROXIMATE) {
            double theta = static_cast<int>(request->parameter());
            AdvancedADCEnum adc_enum(hyper_graph, output_queue, theta, 4);
            adc_enum.SetMethod(vcc::MMCS::Method::MAX);
            adc_enum.SetToQueue(true);
            adc_enum.SetToLog(false);
            adc_enum.RunApproximate();
        }
        done = true;
        child.join();
        std::cout << "Finish processing request\n";
        return Status::OK;
    }
};

void RunServer() {
    std::string server_address("0.0.0.0:50051");
    VertexCoverGeneratorServiceImpl service;

    grpc::EnableDefaultHealthCheckService(true);
    grpc::reflection::InitProtoReflectionServerBuilderPlugin();
    ServerBuilder builder;
    // Listen on the given address without any authentication mechanism.
    builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
    // Register "service" as the instance through which we'll communicate with
    // clients. In this case it corresponds to an *synchronous* service.
    builder.RegisterService(&service);
    // Finally assemble the server.
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server listening on " << server_address << std::endl;

    // Wait for the server to shutdown. Note that some other thread must be
    // responsible for shutting down the server for this call to ever return.
    server->Wait();
}

int main(int argc, char** argv) {
    RunServer();

    return 0;
}
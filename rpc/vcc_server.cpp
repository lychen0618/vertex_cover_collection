#include <grpc/grpc.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>

#include "protos/vertex_cover.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::ServerReader;
using grpc::ServerReaderWriter;
using grpc::ServerWriter;
using grpc::Status;

using vcc::GenRequest;
using vcc::GenResponse;
using vcc::VertexCoverGenerator;

class VertexCoverGeneratorServiceImpl final
    : public VertexCoverGenerator::Service {
    Status Generate(ServerContext* context,
                    const GenRequest* request,
                    ServerWriter<GenResponse>* writer) override {
        // Just fill for run.
        if (request->multi_thread()) {
            for (int i = 0; i < 2; ++i) {
                GenResponse response;
                response.add_time_stamp(i);
                writer->Write(response);
            }
        }
        // TODO:
        // Turn over the request to processor, then write the results to client.
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
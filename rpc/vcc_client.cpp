#include <gflags/gflags.h>
#include <grpcpp/grpcpp.h>

#include <iostream>

#include "protos/vertex_cover.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;

using vcc::GenRequest;
using vcc::GenResponse;
using vcc::RequestType;
using vcc::ValueIntList;
using vcc::VertexCoverGenerator;
#include <vector>

DEFINE_string(type, "", "diversity or approximation");
DEFINE_int32(k, 2, "parameter for diversity");
DEFINE_double(theta, 0.1, "parameter for appro");

class VertexCoverGeneratorClient {
public:
    VertexCoverGeneratorClient(std::shared_ptr<Channel> channel)
        : stub_(VertexCoverGenerator::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string Generate(RequestType type, double para) {
        // Data we are sending to the server.
        GenRequest request;
        request.set_request_type(type);
        request.set_parameter(para);
        std::vector<std::vector<int>> input = {
            {1, 3, 5}, {2, 4, 6}, {2, 3, 4}, {1, 4, 6}};

        for (int i = 0; i < int(input.size()); ++i) {
            ValueIntList* edge = request.mutable_hyper_graph()->add_edge();
            for (int j = 0; j < int(input[i].size()); ++j)
                edge->add_value(input[i][j]);
        }

        // Container for the data we expect from the server.
        GenResponse reply;

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        std::unique_ptr<ClientReader<GenResponse>> reader(
            stub_->Generate(&context, request));
        while (reader->Read(&reply)) {
            for (int i = 0; i < reply.vertex_cover_list().vertex_cover_size();
                 ++i) {
                const ValueIntList& vc =
                    reply.vertex_cover_list().vertex_cover(i);
                for (int j = 0; j < vc.value_size(); ++j) {
                    std::cout << vc.value(j) << " ";
                }
                std::cout << "\n";
            }
        }
        Status status = reader->Finish();

        // Act upon its status.
        if (!status.ok()) {
            std::cout << status.error_code() << ": " << status.error_message()
                      << std::endl;
            return "RPC failed";
        } else
            return "RPC success";
    }

private:
    std::unique_ptr<VertexCoverGenerator::Stub> stub_;
};

int main(int argc, char** argv) {
    // Instantiate the client. It requires a channel, out of which the actual
    // RPCs are created. This channel models a connection to an endpoint
    // specified by the argument "--target=" which is the only expected
    // argument. We indicate that the channel isn't authenticated (use of
    // InsecureChannelCredentials()).
    gflags::ParseCommandLineFlags(&argc, &argv, true);
    VertexCoverGeneratorClient greeter(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
    RequestType type = (FLAGS_type == "diversity" ? RequestType::DIVERSITY
                                                  : RequestType::APPROXIMATE);
    std::cout << "Client start\n";
    std::string reply = greeter.Generate(
        type, (type == RequestType::DIVERSITY) ? FLAGS_k : FLAGS_theta);
    std::cout << "Client end: " << reply << std::endl;

    return 0;
}
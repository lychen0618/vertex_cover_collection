#include <grpcpp/grpcpp.h>

#include <iostream>

#include "protos/vertex_cover.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::ClientReader;
using grpc::Status;

using vcc::GenRequest;
using vcc::GenResponse;
using vcc::VertexCoverGenerator;

class VertexCoverGeneratorClient {
public:
    VertexCoverGeneratorClient(std::shared_ptr<Channel> channel)
        : stub_(VertexCoverGenerator::NewStub(channel)) {}

    // Assembles the client's payload, sends it and presents the response back
    // from the server.
    std::string Generate() {
        // Data we are sending to the server.
        GenRequest request;
        request.set_multi_thread(true);

        // Container for the data we expect from the server.
        GenResponse reply;

        // Context for the client. It could be used to convey extra information
        // to the server and/or tweak certain RPC behaviors.
        ClientContext context;

        // The actual RPC.
        std::unique_ptr<ClientReader<GenResponse>> reader(
            stub_->Generate(&context, request));
        while (reader->Read(&reply)) {
            std::cout << reply.time_stamp_size() << " " << reply.time_stamp(0)
                      << std::endl;
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
    VertexCoverGeneratorClient greeter(grpc::CreateChannel(
        "localhost:50051", grpc::InsecureChannelCredentials()));
    std::string reply = greeter.Generate();
    std::cout << "Greeter received: " << reply << std::endl;

    return 0;
}
#include "generate_processor.h"

namespace vcc {
GenerateProcessor::GenerateProcessor(
    std::shared_ptr<GenResponseQueue> req_queue)
    : req_queue_(req_queue) {}

GenerateProcessor::Process(const GenRequest* const req) {
    if (!Algorithm_IsValid(req->algorithm) ||
        !RequestType_IsValid(req->request_type)) {
        LOG(ERROR) << "Unsupported Algorithm or RequestType";
        return;
    }
    if (req->algorithm() == Algorithm::MMCS) {
        if (req->request_type == RequestType::ALL) {
            ;
        } else if (req->request_type == RequestType::DIVERSITY){
            ;
        }
        else{
            ;
        }
    }
}
}  // namespace vcc
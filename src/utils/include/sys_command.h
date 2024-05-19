#include <string>
#include <grpcpp/grpcpp.h>
#include "command.grpc.pb.h"

void run_system_terminal(::grpc::ServerReaderWriter< ::CLOutput, ::CLInput>* stream);
#include "do-context.h"

#include <capnp/message.h>
#include <kj/array.h>
#include <kj/debug.h>
#include <kj/exception.h>

namespace edgeworker {

void DOContext::log(kj::LogSeverity logSev, kj::String messages) {
  // TODO replace with custom output to stderr formatted in a way so syslog can capture it and send to elasticsearch
  KJ_LOG(ERROR, messages);
}

rpc::DOContext::Reader DOContext::toCapnpReader() {
  capnp::MallocMessageBuilder ctxMsgBuilder;
  // Should I use a capnp orphan here instead?
  auto ctxBuilder = ctxMsgBuilder.initRoot<rpc::DOContext>();
  ctxBuilder.setCorrelationId(correlationId);
  auto tagsList = ctxBuilder.initTags(tags.size());
  auto currentCapnpTag = tagsList.begin();
  for (auto& tag : tags) {
    currentCapnpTag->setKey(tag.key);
    currentCapnpTag->setValue(tag.value);
    currentCapnpTag++;
  };

  return ctxBuilder.asReader();
}

DOContext DOContext::branchWithTag(kj::String key, kj::String value) {
  kj::HashMap<kj::String, kj::String> branchTags {};
  branchTags.insert(kj::str(key), kj::str(value));
  for(auto& tag : this->tags) {
    branchTags.insert(kj::str(tag.key), kj::str(tag.value));
  }
  return DOContext(kj::str(this->correlationId), kj::mv(branchTags));
}

DOContext DOContext::branchWithTags(kj::HashMap<kj::String, kj::String> addedTags) {
  kj::HashMap<kj::String, kj::String> branchTags {};
  for(auto& tag : addedTags) {
    branchTags.insert(kj::str(tag.key), kj::str(tag.value));
  }
  for(auto& tag : this->tags) {
    branchTags.insert(kj::str(tag.key), kj::str(tag.value));
  }
  return DOContext(kj::str(this->correlationId), kj::mv(branchTags));
}

}  // namespace edgeworker

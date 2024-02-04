#pragma once

#include <kj/array.h>
#include <kj/async.h>
#include <kj/async-io.h>
#include <kj/debug.h>
#include <kj/map.h>
#include <workerd/io/do-context.capnp.h>
#include <workerd/util/uuid.h>

namespace edgeworker {

struct DOContext {

  DOContext() {
    correlationId = workerd::randomUUID(kj::None());
  }

  DOContext(kj::String correlationId, kj::HashMap<kj::String, kj::String> tags)
    : correlationId(kj::mv(correlationId)),
    tags(kj::mv(tags))
  {}

  DOContext(kj::String correlationId) {
    this->correlationId = kj::mv(correlationId);
  }

  DOContext(rpc::DOContext::Reader DOContextReader) {
    correlationId = kj::str(DOContextReader.getCorrelationId());
    auto tagsToRead = DOContextReader.getTags();
    for (auto tag : tagsToRead) {
      this->tags.insert(kj::str(tag.getKey()), kj::str(tag.getValue()));
    }
  }

  static DOContext DOContextEmptyId() {
    // empty DOContext. Just for testing
    return DOContext(kj::str(nullptr));
  }

  rpc::DOContext::Reader toCapnpReader();

  DOContext branchWithTag(kj::String key, kj::String value);
  DOContext branchWithTags(kj::HashMap<kj::String, kj::String>);

  void log(kj::LogSeverity logSeverity, kj::String message);

  kj::String correlationId;
  kj::HashMap<kj::String, kj::String> tags = kj::HashMap<kj::String, kj::String>();

};

}  // namespace edgeworker

@0xa724f921c79a8df3;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("edgeworker::rpc");
$Cxx.allowCancellation;

struct DOContext {
  # Object to be passed aroundin RPC calls containing extra info about the request.
  # Primarily useful to enrich logs

  correlationId @0 :Text = "default_correlationID";
  # Id that ties different logs corresponding to a single request together.

  tags @1 :List(Tag);

  struct Tag {
    key @0 :Text;
    value @1 :Text;
  }

}


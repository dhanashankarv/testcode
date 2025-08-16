#include <stdio.h>
enum rdvm_ioreq_src {
	IOREQ_SRC_LOCAL	= 0,
	IOREQ_SRC_RPC	= 1,
	IOREQ_SRC_MAX
};

enum rdvm_ioreq_iotype {
	IOREQ_TYPE_READ			= 1,
	IOREQ_TYPE_WRITE		= 2,
	IOREQ_TYPE_DISCARDBLOCKS	= 3,
	IOREQ_TYPE_REPLWRITE		= 4,
	IOREQ_TYPE_BLOBWRITE		= 5,
	IOREQ_TYPE_INVALID
};
#define IOREQ_NUMTYPES  (IOREQ_TYPE_INVALID - 1)

enum rdvm_ioreq_action {
	IOREQ_ACTION_VALIDATE		= 0,
	IOREQ_ACTION_VOLUME_LOAD	= 1,
	IOREQ_ACTION_MNTGENID_CHECK	= 2,
	IOREQ_ACTION_SLICE_LOAD		= 3,
	IOREQ_ACTION_SLICE_GENUPDATE	= 4,
	IOREQ_ACTION_SNAPSHOT_LOAD	= 5,
	IOREQ_ACTION_BMAP_REF		= 6,
	IOREQ_ACTION_BMAP_LOAD		= 7,
	IOREQ_ACTION_BMAP_LOOKUP	= 8,
	IOREQ_ACTION_SEGMENT_ASSIGN	= 9,
	IOREQ_ACTION_REPLICATE_ISSUE	= 10,
	IOREQ_ACTION_DEV_IOSUBMIT	= 11,
	IOREQ_ACTION_REDO_ADDENTRY	= 12,
	IOREQ_ACTION_REDO_FLUSH		= 13,
	IOREQ_ACTION_DEV_IOCOMPLETE	= 14,
	IOREQ_ACTION_BMAP_UPDATE	= 15,
	IOREQ_ACTION_SEG_ACKQUEUE_WAIT	= 16,
	IOREQ_ACTION_COMPLETE_CB	= 17,
	IOREQ_ACTION_INVALID
};
#define IOREQ_NUMACTIONS	(IOREQ_TYPE_INVALID)

enum rdvm_ioreq_state {
	IOREQ_STATE_INITIALIZED	= 1,
	IOREQ_STATE_ACTIVE	= 2,
	IOREQ_STATE_QUEUED	= 3,
	IOREQ_STATE_ISSUED	= 4,
	IOREQ_STATE_FAILED	= 5,
	IOREQ_STATE_COMPLETED	= 6,
	IOREQ_STATE_MAX
};

enum rdvm_ioreq_action rdvm_ioreq_actseq[IOREQ_NUMTYPES][IOREQ_NUMACTIONS] = {
	[IOREQ_TYPE_READ] = {
	IOREQ_ACTION_VALIDATE,
	IOREQ_ACTION_VOLUME_LOAD,
	IOREQ_ACTION_MNTGENID_CHECK,
	},
	[IOREQ_TYPE_WRITE] = {
	IOREQ_ACTION_VALIDATE,
	IOREQ_ACTION_VOLUME_LOAD,
	IOREQ_ACTION_MNTGENID_CHECK,
	},
};

struct rdvm_ioreq;
typedef void (*ioreq_reissue_fn)(struct rdvm_ioreq *req, void *priv);
typedef void (*ioreq_fail_fn)(struct rdvm_ioreq *item, void *priv, int status);
struct rdvm_ioreq {
	//struct zlist_node	chain;
	struct rdvm_env		*env;
	enum rdvm_ioreq_iotype	type;
	enum rdvm_ioreq_src	src;
	enum rdvm_ioreq_action 	action;
	enum rdvm_ioreq_state 	state;
	struct io_attrs		*io_attrs;
	void			*attrs;
	void			*priv; // thread specific
	//cpr_atomic_t		refcnt;
	ioreq_reissue_fn	requeue;
	ioreq_fail_fn		fail;

	struct rdvm_volume	*vol;
	struct rdvm_dev		*dev;
	struct rdvm_slice	*slice;
	struct rdvm_snapshot	*snap;
};

typedef int (*ioreq_handler)(struct rdvm_ioreq *req);
struct rdvm_ioreq_handlers {
	ioreq_handler		handler;
};

int rdvm_ioreq_rpc_read(struct rdvm_ioreq *req);
int rdvm_ioreq_rpc_write(struct rdvm_ioreq *req);
int rdvm_ioreq_rpc_discardblocks(struct rdvm_ioreq *req);
int rdvm_ioreq_rpc_replwrite(struct rdvm_ioreq *req);
int rdvm_ioreq_rpc_segblobwrite(struct rdvm_ioreq *req);
int rdvm_ioreq_local_read(struct rdvm_ioreq *req);
int rdvm_ioreq_local_write(struct rdvm_ioreq *req);
int rdvm_ioreq_local_discardblocks(struct rdvm_ioreq *req);

struct rdvm_ioreq_handlers ioreq_handlers[IOREQ_SRC_MAX][IOREQ_NUMTYPES] = {
	[IOREQ_SRC_RPC][IOREQ_TYPE_READ] = {
		.handler = rdvm_ioreq_rpc_read,
	},
	[IOREQ_SRC_RPC][IOREQ_TYPE_WRITE] = {
		.handler = rdvm_ioreq_rpc_write,
	},
	[IOREQ_SRC_RPC][IOREQ_TYPE_DISCARDBLOCKS] = {
		.handler = rdvm_ioreq_rpc_discardblocks,
	},
	[IOREQ_SRC_RPC][IOREQ_TYPE_READ] = {
		.handler = rdvm_ioreq_rpc_read,
	},
	[IOREQ_SRC_RPC][IOREQ_TYPE_WRITE] = {
		.handler = rdvm_ioreq_rpc_write,
	},
	[IOREQ_SRC_RPC][IOREQ_TYPE_DISCARDBLOCKS] = {
		.handler = rdvm_ioreq_rpc_discardblocks,
	},
};

void
rdvm_ioreq_setup_actions(void)
{
	int action = 0;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_VOLUME_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_SLICE_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_SLICE_GENUPDATE;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_SNAPSHOT_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_BMAP_REF;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_BMAP_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_BMAP_LOOKUP;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_DEV_IOSUBMIT;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_DEV_IOCOMPLETE;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_COMPLETE_CB;
	rdvm_ioreq_actseq[IOREQ_TYPE_READ][action++] = IOREQ_ACTION_INVALID;

	action = 0;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_VOLUME_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_MNTGENID_CHECK;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_SLICE_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_SLICE_GENUPDATE;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_SNAPSHOT_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_BMAP_REF;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_BMAP_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_SEGMENT_ASSIGN;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_REPLICATE_ISSUE;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_DEV_IOSUBMIT;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_REDO_ADDENTRY;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_REDO_FLUSH;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_DEV_IOCOMPLETE;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_BMAP_UPDATE;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_SEG_ACKQUEUE_WAIT;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_COMPLETE_CB;
	rdvm_ioreq_actseq[IOREQ_TYPE_WRITE][action++] = IOREQ_ACTION_INVALID;

	action = 0;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_VOLUME_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_MNTGENID_CHECK;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_SLICE_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_SLICE_GENUPDATE;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_SNAPSHOT_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_BMAP_REF;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_BMAP_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_BMAP_LOOKUP;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_SEGMENT_ASSIGN;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_REPLICATE_ISSUE;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_REDO_ADDENTRY;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_REDO_FLUSH;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_SEG_ACKQUEUE_WAIT;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_COMPLETE_CB;
	rdvm_ioreq_actseq[IOREQ_TYPE_DISCARDBLOCKS][action++] = IOREQ_ACTION_INVALID;

	action = 0;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_VOLUME_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_MNTGENID_CHECK;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_SLICE_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_SLICE_GENUPDATE;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_SEGMENT_ASSIGN;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_REDO_ADDENTRY;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_REDO_FLUSH;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_COMPLETE_CB;
	rdvm_ioreq_actseq[IOREQ_TYPE_REPLWRITE][action++] = IOREQ_ACTION_INVALID;

	action = 0;
	rdvm_ioreq_actseq[IOREQ_TYPE_BLOBWRITE][action++] = IOREQ_ACTION_VOLUME_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_BLOBWRITE][action++] = IOREQ_ACTION_MNTGENID_CHECK;
	rdvm_ioreq_actseq[IOREQ_TYPE_BLOBWRITE][action++] = IOREQ_ACTION_SLICE_LOAD;
	rdvm_ioreq_actseq[IOREQ_TYPE_BLOBWRITE][action++] = IOREQ_ACTION_SLICE_GENUPDATE;
	rdvm_ioreq_actseq[IOREQ_TYPE_BLOBWRITE][action++] = IOREQ_ACTION_SEGMENT_ASSIGN;
	rdvm_ioreq_actseq[IOREQ_TYPE_BLOBWRITE][action++] = IOREQ_ACTION_COMPLETE_CB;
	rdvm_ioreq_actseq[IOREQ_TYPE_BLOBWRITE][action++] = IOREQ_ACTION_INVALID;
};

void
main()
{
	rdvm_ioreq_setup_actions();
	return;
}

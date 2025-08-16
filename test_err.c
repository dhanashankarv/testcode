#include <stdio.h>
#include <errno.h>

#define ERR_FAILED		500	/* something has failed */
#define ERR_ACCESS		501	/* access denied for something */
#define ERR_PERM		502	/* don't have permission on something */
#define ERR_FAULT		503	/* some fault occured */
#define ERR_REFUSED		504	/* operation was refused */
#define ERR_NOTFOUND		505	/* something was not found */
#define ERR_EXISTS		506	/* something already exists */
#define ERR_DUPLICATE		507	/* duplicate entry found */
#define ERR_MISSING		508	/* something is missing */
#define ERR_INVALID		509	/* something is invalid */
#define ERR_MEMORY		510	/* ran out of memory / no more memory */
#define ERR_NOSPACE		511	/* no more space available */
#define ERR_NODATA		512	/* no data found, when expected */
#define ERR_EMPTY		513	/* something is empty */
#define ERR_MAX			514	/* max value reached */
#define ERR_OVERFLOW		515	/* overflow error */
#define ERR_UNDERFLOW		516	/* underflow error */
#define ERR_BUFSIZE		517	/* buffer exceeded its size */
#define ERR_BUSY		518	/* resource is busy */
#define ERR_NOTREADY		519	/* something is not ready */
#define ERR_INUSE		520	/* something is already in use */
#define ERR_RETRY		521	/* retry last operation */
#define ERR_ALIGN		522	/* something is not aligned */
#define ERR_SYNTAX		523	/* syntax error */
#define ERR_SEMANTIC		524	/* semantic error */
#define ERR_PARSE		525	/* parse error */
#define ERR_BADXDM		526	/* bad XDM error */
#define ERR_SYSCALL		527	/* system call error (check errno) */
#define ERR_IO			528	/* IO error */
#define ERR_SIZE		529	/* bad size */
#define ERR_NEEDACTION		530	/* needs action */
#define ERR_DEADLOCK		531	/* operation will cause deadlock */
#define ERR_NOTALLOWED		532	/* operation is not allowed */
#define ERR_MISMATCH		533	/* parameter or version mismatch */
#define ERR_TOOLONG		534	/* parameter or record is too long */
#define ERR_SQLITE		535	/* a SQLITE DB error occured */
#define ERR_NOTEMPTY		536	/* directory or resource is not empty */
#define ERR_NOTDIR		537	/* component is not a directory */
#define ERR_MALFORMED		538	/* data or record is malformed */
#define ERR_UNKNOWN		539	/* unknown value */
#define ERR_BADOP		540	/* bad opcode */
#define ERR_ALREADY		541	/* operation already performed */
#define ERR_TIMEOUT		542	/* operation timedout */
#define ERR_PROTO		543	/* protocol error */
#define ERR_DISCONN		544	/* resource disconnected */
#define ERR_USAGE		545	/* incorrect usage */
#define ERR_INTERN		546	/* internal error */
#define ERR_ERESET		547	/* current working directory mismatch */
#define ERR_LOOP		548	/* link will result in a loop */
#define ERR_RANGE		549	/* Out of range */
#define ERR_NOENTRY		550	/* entry doesn't exist */
#define ERR_RPC			551	/* RPC error */
#define ERR_STALE		552	/* stale */
#define ERR_TXTBUSY		553	/* text area/buffer is busy/in-use */
#define ERR_AGAIN		554	/* Try command/operation again */
#define ERR_BLOCKED		555	/* Request for resource is blocked */
#define ERR_WOULDBLOCK		556	/* Request would block */
#define ERR_NOSUPPORT		557	/* Operation not supported */
#define ERR_CHECKSUM		558	/* Checksum did not match */
#define ERR_READONLY		559	/* Modifying a readonly resource */
#define ERR_INPROGRESS		560	/* Operation is in progress */
#define ERR_PARTIAL		561	/* Partial Data Available */
#define ERR_DECODE		562	/* JSON Decode error */
#define ERR_ENCODE		563	/* JSON Encode error */
#define ERR_DELETED		564	/* object has been deleted */
#define ERR_JANSSON		565	/* Jannson JSON parsing error */
#define ERR_NOTBLK		566	/* Not a block device */
#define ERR_TOOBIG		567	/* Record is too big */
#define ERR_FSIM		568	/* Simulated Fault */
#define ERR_MAP			569	/* Slice Map related faults */
#define ERR_CANCEL		570	/* Operation was canceled */
#define ERR_NOTMOUNTED		571	/* Object not mounted */
#define ERR_SM_CONNECT   	572	/* Not able to connect to SM */
#define ERR_DETACHED		573	/* Detached from the parent */
static int err_codes[] = {
        [ERR_FAILED] = ERR_FAILED,      [ERR_ACCESS] = EACCES,
        [ERR_PERM] = EPERM,             [ERR_FAULT] = EFAULT,
        [ERR_REFUSED] = ERR_REFUSED,    [ERR_NOTFOUND] = ENOENT,
        [ERR_EXISTS] = EEXIST,          [ERR_DUPLICATE] = ERR_DUPLICATE,
        [ERR_MISSING] = ERR_MISSING,    [ERR_INVALID] = EINVAL,
        [ERR_MEMORY] = ENOMEM,          [ERR_NOSPACE] = ENOSPC,
        [ERR_NODATA] = ENODATA,         [ERR_EMPTY] = ERR_EMPTY,
        [ERR_MAX] = ERR_MAX,            [ERR_OVERFLOW] = EOVERFLOW,
        [ERR_UNDERFLOW] = ERR_UNDERFLOW,[ERR_BUFSIZE] = EMSGSIZE,
        [ERR_BUSY] = EBUSY,             [ERR_NOTREADY] = ERR_NOTREADY,
        [ERR_INUSE] = EADDRINUSE,       [ERR_RETRY] = ERR_RETRY,
        [ERR_ALIGN] = ERR_ALIGN,        [ERR_SYNTAX] = ERR_SYNTAX,
        [ERR_SEMANTIC] = ERR_SEMANTIC,  [ERR_PARSE] = ERR_PARSE,
        [ERR_BADXDM] = ERR_BADXDM,      [ERR_SYSCALL] = ERR_SYSCALL,
        [ERR_IO] = EIO,			[ERR_SIZE] = ERR_SIZE,
        [ERR_NEEDACTION] = ERR_NEEDACTION, [ERR_DEADLOCK] = EDEADLOCK,
        [ERR_NOTALLOWED] = ERR_NOTALLOWED, [ERR_MISMATCH] = ERR_MISMATCH,
        [ERR_TOOLONG] = ENAMETOOLONG,   [ERR_SQLITE] = ERR_SQLITE,
        [ERR_NOTEMPTY] = ENOTEMPTY,     [ERR_NOTDIR] = ENOTDIR,
        [ERR_MALFORMED] = EBADMSG,      [ERR_UNKNOWN] = ERR_UNKNOWN,
        [ERR_BADOP] = EOPNOTSUPP,       [ERR_ALREADY] = ERR_ALREADY,
        [ERR_TIMEOUT] = ERR_TIMEOUT,    [ERR_PROTO] = EPROTO,
        [ERR_DISCONN] = ENOTCONN,       [ERR_USAGE] = ERR_USAGE,
        [ERR_INTERN] = ERR_INTERN,      [ERR_ERESET] = ERR_ERESET,
        [ERR_LOOP] = ELOOP,             [ERR_RANGE] = ERANGE,
        [ERR_NOENTRY] = ERR_NOENTRY,    [ERR_RPC] = ERR_RPC,
        [ERR_STALE] = ESTALE,           [ERR_TXTBUSY] = ETXTBSY,
        [ERR_AGAIN] = EAGAIN,           [ERR_BLOCKED] = ERR_BLOCKED,
        [ERR_WOULDBLOCK] = EWOULDBLOCK, [ERR_NOSUPPORT] = ERR_NOSUPPORT,
        [ERR_CHECKSUM] = ERR_CHECKSUM,  [ERR_READONLY] = ERR_READONLY,
        [ERR_INPROGRESS] = EINPROGRESS
};
static int err_codes_max = sizeof (err_codes)/sizeof (err_codes[0]);

static int errno_mappings[] = {
	[EPERM]		= ERR_PERM,
	[ENOENT]	= ERR_NOTFOUND,
	[EIO]		= ERR_IO,
	[E2BIG]		= ERR_TOOBIG,
	[EAGAIN]	= ERR_RETRY,
	[ENOMEM] 	= ERR_MEMORY,
	[EACCES]	= ERR_PERM,
	[EFAULT]	= ERR_FAULT,
	[ENOTBLK]	= ERR_NOTBLK,
	[EBUSY]		= ERR_BUSY,
	[EEXIST]	= ERR_EXISTS,
	[ENODEV]	= ERR_NOTFOUND,
	[ENOTDIR]	= ERR_NOTDIR,
	[EINVAL]	= ERR_INVALID,
	[ENOSPC]	= ERR_NOSPACE,
	[EOVERFLOW]	= ERR_OVERFLOW,
	[ETIMEDOUT]	= ERR_TIMEOUT,
	[ENOTSUP]	= ERR_NOSUPPORT,
	[ECONNREFUSED]  = ERR_REFUSED,
	[ESTALE]	= ERR_STALE,
	[ECANCELED]	= ERR_CANCEL,
};
static inline int
errno_to_errcode(int e)
{
	static int errno_mappings_max = sizeof (errno_mappings)/sizeof(errno_mappings[0]);
	if (e > errno_mappings_max) {
		return e;
	}
	return errno_mappings[e] ? errno_mappings[e] : 0;
}
int
err_to_errno(int error)
{
	if (error >= 0 && error < err_codes_max) {
		return (err_codes[error] ? err_codes[error] : error);
	}
	return error;
}

int
main(int argc, char *argv[])
{
	int err = errno_to_errcode(ENOENT);
	printf("%d --> %d\n", ENOENT, err);
	err = err_to_errno(ERR_NOTFOUND);
	printf("%d --> %d\n", ERR_NOTFOUND, err);
	return 0;
}

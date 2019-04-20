#ifndef USERFS_H
#define USERFS_H

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/**
 * User-defined in-memory filesystem. It is as simple as possible.
 * Each file lies in the memory as an array of blocks. A file
 * has an unique file name, and there are no directories, so the
 * FS is a monolite flat contiguous folder.
 */

/**
 * Flags for ufs_open call.
 */
enum open_flags {
	/**
	 * If the flag specified and a file does not exist -
	 * create it.
	 */
	UFS_CREATE    	 = 1 << 0,
	UFS_READ_ONLY 	 = 1 << 1,
	UFS_WRITE_ONLY 	 = 1 << 2,
	UFS_READ_WRITE 	 = 1 << 3,
};

/** Possible errors from all functions. */
enum ufs_error_code {
	UFS_ERR_NO_ERR = 0,
	UFS_ERR_NO_MEM,
	UFS_ERR_NO_FILE,
	UFS_ERR_NO_PERMISSION,
	UFS_ERR_NO_DESC,
};

/** Get code of the last error. */
enum ufs_error_code
ufs_errno();

/**
 * Open a file by filename.
 * @param filename Name of a file to open.
 * @param flags Bitwise combination of open_flags.
 *
 * @retval > 0 File descriptor.
 * @retval -1 Error occured. Check ufs_errno() for a code.
 *     - UFS_ERR_NO_FILE - no such file, and UFS_CREATE flag is
 *       not specified.
 */
int
ufs_open(const char *filename, int flags);

/**
 * Write data to the file.
 * @param fd File descriptor from ufs_open().
 * @param buf Buffer to write.
 * @param size Size of @a buf.
 *
 * @retval > 0 How many bytes were written.
 * @retval -1 Error occured. Check ufs_errno() for a code.
 *     - UFS_ERR_NO_FILE - invalid file descriptor.
 *     - UFS_ERR_NO_MEM - not enough memory.
 */
ssize_t
ufs_write(int fd, const char *buf, size_t size);

/**
 * Read data from the file.
 * @param fd File descriptor from ufs_open().
 * @param buf Buffer to read into.
 * @param size Maximum bytes to read.
 *
 * @retval > 0 How many bytes were read.
 * @retval 0 EOF.
 * @retval -1 Error occured. Check ufs_errno() for a code.
 *     - UFS_ERR_NO_FILE - invalid file descriptor.
 */
ssize_t
ufs_read(int fd, char *buf, size_t size);

/**
 * Close a file.
 * @param fd File descriptor from ufs_open().
 * @retval 0 Success.
 * @retval -1 Error occured. Check ufs_errno() for a code.
 *     - UFS_ERR_NO_FILE - invalid file descriptor.
 */
int
ufs_close(int fd);

/**
 * Delete a file by its name.
 * @param filename Name of a file to delete.
 * @retval -1 Error occured. Check ufs_errno() for a code.
 *     - UFS_ERR_NO_FILE - no such file.
 */
int
ufs_delete(const char *filename);

/**
 * free all fs memory
 */
void
ufs_free(void);

/**
 * print all fs data
 */
void ufs_print(void);

#endif


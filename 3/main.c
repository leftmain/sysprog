#include "userfs.h"

int main(void) {
	const char *buf = "0123456789012345678901234567890123456789";
	char s[1024];
	int fd1 = 0;
	int fd2 = 0;
	int fd3 = 0;
	int fd4 = 0;
	int fd5 = 0;
	int fd6 = 0;
	int t = 1;

	printf("*** Test%d ***\n", t++);
	fd1 = ufs_open("file1", UFS_READ_ONLY);
	if (fd1 == -1)
		printf("errno: %d\n", ufs_errno());

	printf("\n*** Test%d ***\n", t++);
	fd1 = ufs_open("file1", UFS_CREATE | UFS_READ_WRITE);
	if (fd1 == -1)
		printf("errno: %d\n", ufs_errno());
	for (int i = 0; i < 100; i++) ufs_write(fd1, buf, 20);
	ufs_print();

	printf("\n*** Test%d ***\n", t++);
	fd1 = ufs_open("file1", UFS_WRITE_ONLY);
	fd2 = ufs_open("file1", UFS_READ_ONLY);
	for (int i = 0; i < 511; i++) ufs_write(fd1, "0", 1);
	ufs_print();

	printf("\n*** Test%d ***\n", t++);
	for (int i = 0; i < 10; i++) {
		ufs_read(fd2, s, 100);
		write(1, s, 100);
		write(1, "\n", 1);
	}
	ufs_delete("file1");
	ufs_print();

	printf("\n*** Test%d ***\n", t++);
	fd1 = ufs_open("file0", UFS_CREATE | UFS_READ_ONLY);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	ufs_write(fd1, buf, 1);
	fd1 = ufs_open("file1", UFS_CREATE | UFS_WRITE_ONLY);
	ufs_write(fd1, buf, 2);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	fd1 = ufs_open("file2", UFS_CREATE | UFS_READ_WRITE);
	ufs_write(fd1, buf, 3);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	fd1 = ufs_open("file3", UFS_CREATE | UFS_READ_WRITE);
	ufs_write(fd1, buf, 4);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	fd1 = ufs_open("file4", UFS_CREATE | UFS_READ_WRITE);
	ufs_write(fd1, buf, 5);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	fd1 = ufs_open("file5", UFS_CREATE | UFS_READ_WRITE);
	ufs_write(fd1, buf, 6);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	fd1 = ufs_open("file6", UFS_CREATE | UFS_READ_WRITE);
	ufs_write(fd1, buf, 7);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	fd1 = ufs_open("file7", UFS_CREATE | UFS_READ_WRITE);
	ufs_write(fd1, buf, 8);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	fd1 = ufs_open("file8", UFS_CREATE | UFS_READ_WRITE);
	ufs_write(fd1, buf, 8);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	fd1 = ufs_open("file9", UFS_CREATE | UFS_READ_WRITE);
	ufs_write(fd1, buf, 10);
	if (fd1 == -1) printf("errno: %d\n", ufs_errno());
	ufs_print();

	printf("\n*** Test%d ***\n", t++);
	ufs_close(fd1);
	ufs_delete("file1");
	ufs_delete("file3");
	ufs_delete("file5");
	ufs_delete("file7");
	ufs_delete("file9");
	ufs_print();

	printf("\n*** Test%d ***\n", t++);
	fd1 = ufs_open("file1", UFS_CREATE | UFS_READ_WRITE);
	fd2 = ufs_open("file1", UFS_CREATE | UFS_READ_WRITE);
	fd3 = ufs_open("file1", UFS_CREATE | UFS_READ_WRITE);
	fd4 = ufs_open("file1", UFS_CREATE | UFS_READ_WRITE);
	fd5 = ufs_open("file1", UFS_CREATE | UFS_READ_WRITE);
	fd6 = ufs_open("file1", UFS_CREATE | UFS_READ_WRITE);
	ufs_print();

	printf("\n*** Test%d ***\n", t++);
	ufs_close(fd1);
	ufs_close(fd2);
	ufs_close(fd3);
	ufs_close(fd4);
	ufs_print();

	ufs_free();
	return 0;
}


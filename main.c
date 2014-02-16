// Copyright (c) 2014 Joe Ranieri
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
//   1. The origin of this software must not be misrepresented; you must not
//   claim that you wrote the original software. If you use this software
//   in a product, an acknowledgment in the product documentation would be
//   appreciated but is not required.
//
//   2. Altered source versions must be plainly marked as such, and must not be
//   misrepresented as being the original software.
//
//   3. This notice may not be removed or altered from any source
//   distribution.
//

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

// Hidden under PRIVATE in the kernel's copy of <sys/resource.h>.
#define IOPOL_TYPE_VFS_HFS_CASE_SENSITIVITY 1
#define IOPOL_VFS_HFS_CASE_SENSITIVITY_FORCE_CASE_SENSITIVE 1

static void verify(int ret, const char *msg) {
  if (ret) {
    perror(msg);
    exit(EXIT_FAILURE);
  }
}

int main(int argc, char *const argv[]) {
  if (argc < 2) {
    fprintf(stderr, "usage: %s executable [arg1 [arg2 [...]]]\n",
            getprogname());
    return EXIT_FAILURE;
  }

  // Set the policy to force case sensitive lookups via a private io policy,
  // which unfortunately requires to be running as root. Since it only applies
  // to this process, I don't know why -- see iopolicysys_vfs in xnu.
  int err =
      setiopolicy_np(IOPOL_TYPE_VFS_HFS_CASE_SENSITIVITY, IOPOL_SCOPE_PROCESS,
                     IOPOL_VFS_HFS_CASE_SENSITIVITY_FORCE_CASE_SENSITIVE);
  verify(err, "setiopolicy_np");

  // If running under sudo, switch us to be the real user instead of running
  // it as root.
  const char *sudo_gid = getenv("SUDO_GID");
  if (sudo_gid) {
    err = setgid((gid_t)atoi(sudo_gid));
    verify(err, "setgid");
  }

  const char *sudo_uid = getenv("SUDO_UID");
  if (sudo_uid) {
    err = setuid((uid_t)atoi(sudo_uid));
    verify(err, "setuid");
  }

  // 'shift' all of the arguments over to the left and launch our target.
  size_t arg_bytes = ((unsigned)argc - 1) * sizeof(char *);
  char **new_argv = malloc(arg_bytes);
  memcpy(new_argv, argv + 1, arg_bytes);
  (void)execvp(argv[1], new_argv);

  // Shouldn't make it here
  perror("execlp");
  return EXIT_FAILURE;
}

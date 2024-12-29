/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CGROUP_INTERNAL_H
#define CGROUP_INTERNAL_H

#include "container.h"
#include "utils.h"

enum
{
  CGROUP_MEMORY = 1 << 0,
  CGROUP_CPU = 1 << 1,
  CGROUP_HUGETLB = 1 << 2,
  CGROUP_CPUSET = 1 << 3,
  CGROUP_PIDS = 1 << 4,
  CGROUP_IO = 1 << 5,
  CGROUP_MISC = 1 << 6,
};

struct libcrun_cgroup_status
{
  char *path;
  char *scope;

  int manager;
};

struct libcrun_cgroup_manager
{
  /* Create a new cgroup and fill PATH in OUT.  */
  int (*create_cgroup) (struct libcrun_cgroup_args *args, struct libcrun_cgroup_status *out, libcrun_error_t *err);
  int (*precreate_cgroup) (struct libcrun_cgroup_args *args, int *dirfd, libcrun_error_t *err);
  /* Destroy the cgroup and kill any process if needed.  */
  int (*destroy_cgroup) (struct libcrun_cgroup_status *cgroup_status, libcrun_error_t *err);
  /* Additional resources configuration specific to this manager.  */
  int (*update_resources) (struct libcrun_cgroup_status *cgroup_status, const char *state_root, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);
};

int move_process_to_cgroup (pid_t pid, const char *subsystem, const char *path, libcrun_error_t *err);
int enter_cgroup_subsystem (pid_t pid, const char *subsystem, const char *path, bool create_if_missing,
                            libcrun_error_t *err);
int enable_controllers (const char *path, libcrun_error_t *err);
int chown_cgroups (const char *path, uid_t uid, gid_t gid, libcrun_error_t *err);
int destroy_cgroup_path (const char *path, int mode, libcrun_error_t *err);
int cgroup_killall_path (const char *path, int signal, libcrun_error_t *err);

int libcrun_cgroup_read_pids_from_path (const char *path, bool recurse, pid_t **pids, libcrun_error_t *err);

bool read_proc_cgroup (char *content, char **saveptr, char **id, char **controller_list, char **path);

static inline int
is_rootless (libcrun_error_t *err)
{
  if (geteuid ())
    return 1;

  return check_running_in_user_namespace (err);
}

int libcrun_cgroup_pause_unpause_path (const char *cgroup_path, const bool pause, libcrun_error_t *err);

static inline uint64_t
convert_shares_to_weight (uint64_t shares)
{
  /* convert linearly from 2-262144 to 1-10000.  */
  return (1 + ((shares - 2) * 9999) / 262142);
}

int initialize_cpuset_subsystem (const char *path, libcrun_error_t *err);
int initialize_cpuset_subsystem_resources (const char *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err);

int write_cpuset_resources (int dirfd_cpuset, int cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

int write_cpu_burst (int cpu_dirfd, bool cgroup2, runtime_spec_schema_config_linux_resources_cpu *cpu, libcrun_error_t *err);

#endif

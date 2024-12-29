export VC_LIST_EXCEPT_DEFAULT=^(lib/.*|m4/.*|md5/.*|build-aux/.*|src/gettext\.h|.*ChangeLog|src/libcrun/cloned_binary.c|src/libcrun/chroot_realpath.c|.*\.1)$$

local-checks-to-skip = \
    sc_immutable_NEWS \
    sc_copyright_check \
    \
    sc_prohibit_path_max_allocation \
    sc_prohibit_strcmp \
    sc_program_name \
    sc_bindtextdomain	 \
    sc_unmarked_diagnostics \
    sc_prohibit_intprops_without_use \
    sc_error_message_uppercase \
    sc_GPL_version \
    sc_cast_of_x_alloc_return_value \
    sc_indent \
    sc_prohibit_always-defined_macros \
    sc_prohibit_gnu_make_extensions

#SHELL=bash -x
show-vc-list-except:
	@$(VC_LIST_EXCEPT)

VC_LIST_ALWAYS_EXCLUDE_REGEX = ^ABOUT-NLS|maint.mk|git.mk|tests.*|COPYING|contrib/.*|src/libcrun/blake3/.*$$

export gl_public_submodule_commit=

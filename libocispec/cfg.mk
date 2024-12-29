export VC_LIST_EXCEPT_DEFAULT=^(lib/.*|m4/.*|md5/.*|build-aux/.*|src/gettext\.h|.*ChangeLog)$$

local-checks-to-skip = \
    sc_immutable_NEWS \
    sc_copyright_check \
    \
    sc_program_name \
    sc_bindtextdomain	 \
    sc_error_message_period \
    sc_unmarked_diagnostics \
    sc_GPL_version \
    sc_prohibit_always_true_header_tests \
    sc_prohibit_intprops_without_use \
    sc_prohibit_strcmp \

#SHELL=bash -x
show-vc-list-except:
	@$(VC_LIST_EXCEPT)

VC_LIST_ALWAYS_EXCLUDE_REGEX = ^ABOUT-NLS|maint.mk|git.mk|tests.*|COPYING$$

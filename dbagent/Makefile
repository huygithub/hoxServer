# Your project's name.
PROGRAM = dbagent

# Libraries specific flags.
ST_CXXFLAGS    = -I../include
ST_LDLIBS      =
BOOST_CXXFLAGS =

# Common flags
CXX         = g++
CXXFLAGS    = $(ST_CXXFLAGS) $(BOOST_CXXFLAGS) -Wall -Werror
LDLIBS      = $(ST_LDLIBS) -lpthread -lsqlite3
LDFLAGS     =
#DEBUGFLAGS  = -g
DEBUGFLAGS  =

# Define our sources and object files
SOURCES := \
	hoxLog.cpp \
	hoxDebug.cpp \
	hoxExcept.cpp \
	hoxSocketAPI.cpp \
	hoxUtil.cpp \
	hoxTypes.cpp \
	hoxDBAPI.cpp \
	main.cpp

OBJECTS := $(SOURCES:.cpp=.o)

.cpp.o :
	$(CXX) $(CXXFLAGS) $(DEBUGFLAGS) -c  -o $@ $<

all: $(PROGRAM)

$(PROGRAM): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(PROGRAM) $(OBJECTS) $(LDLIBS)

depend: $(SOURCES)
	makedepend $(CXXFLAGS) $(SOURCES)

clean:
	rm -rf $(PROGRAM) $(OBJECTS) *.bak

zip:
	zip $(PROGRAM) $(SOURCES) *.h Makefile *.sh

# DO NOT DELETE

hoxLog.o: /usr/include/stdio.h /usr/include/features.h
hoxLog.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
hoxLog.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
hoxLog.o: /usr/include/gnu/stubs-32.h /usr/include/bits/types.h
hoxLog.o: /usr/include/bits/typesizes.h /usr/include/libio.h
hoxLog.o: /usr/include/_G_config.h /usr/include/wchar.h
hoxLog.o: /usr/include/bits/stdio_lim.h /usr/include/bits/sys_errlist.h
hoxLog.o: /usr/include/string.h /usr/include/xlocale.h /usr/include/unistd.h
hoxLog.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
hoxLog.o: /usr/include/getopt.h /usr/include/errno.h
hoxLog.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
hoxLog.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
hoxLog.o: /usr/include/asm-generic/errno-base.h /usr/include/time.h
hoxLog.o: /usr/include/bits/time.h hoxLog.h
hoxDebug.o: hoxDebug.h hoxLog.h
hoxExcept.o: hoxExcept.h hoxEnums.h
hoxSocketAPI.o: hoxSocketAPI.h hoxEnums.h hoxLog.h /usr/include/sys/socket.h
hoxSocketAPI.o: /usr/include/features.h /usr/include/bits/predefs.h
hoxSocketAPI.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxSocketAPI.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxSocketAPI.o: /usr/include/sys/uio.h /usr/include/sys/types.h
hoxSocketAPI.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
hoxSocketAPI.o: /usr/include/time.h /usr/include/bits/time.h
hoxSocketAPI.o: /usr/include/xlocale.h /usr/include/endian.h
hoxSocketAPI.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
hoxSocketAPI.o: /usr/include/sys/select.h /usr/include/bits/select.h
hoxSocketAPI.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
hoxSocketAPI.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
hoxSocketAPI.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
hoxSocketAPI.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
hoxUtil.o: /usr/include/boost/tokenizer.hpp
hoxUtil.o: /usr/include/boost/token_iterator.hpp
hoxUtil.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxUtil.o: /usr/include/features.h /usr/include/bits/predefs.h
hoxUtil.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxUtil.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxUtil.o: /usr/include/boost/iterator/iterator_adaptor.hpp
hoxUtil.o: /usr/include/boost/static_assert.hpp /usr/include/boost/config.hpp
hoxUtil.o: /usr/include/boost/config/user.hpp
hoxUtil.o: /usr/include/boost/config/select_compiler_config.hpp
hoxUtil.o: /usr/include/boost/config/compiler/gcc.hpp
hoxUtil.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxUtil.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxUtil.o: /usr/include/boost/config/select_platform_config.hpp
hoxUtil.o: /usr/include/boost/config/posix_features.hpp /usr/include/unistd.h
hoxUtil.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
hoxUtil.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
hoxUtil.o: /usr/include/getopt.h /usr/include/boost/config/suffix.hpp
hoxUtil.o: /usr/include/boost/detail/workaround.hpp
hoxUtil.o: /usr/include/boost/config.hpp /usr/include/boost/iterator.hpp
hoxUtil.o: /usr/include/boost/detail/iterator.hpp
hoxUtil.o: /usr/include/boost/iterator/iterator_categories.hpp
hoxUtil.o: /usr/include/boost/iterator/detail/config_def.hpp
hoxUtil.o: /usr/include/boost/mpl/eval_if.hpp /usr/include/boost/mpl/if.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/value_wknd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/static_cast.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/workaround.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/integral.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/msvc.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/eti.hpp
hoxUtil.o: /usr/include/boost/mpl/int.hpp /usr/include/boost/mpl/int_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/adl_barrier.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/adl.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/intel.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/gcc.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/nttp_decl.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/nttp.hpp
hoxUtil.o: /usr/include/boost/preprocessor/cat.hpp
hoxUtil.o: /usr/include/boost/preprocessor/config/config.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/integral_wrapper.hpp
hoxUtil.o: /usr/include/boost/mpl/integral_c_tag.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/static_constant.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/na_spec.hpp
hoxUtil.o: /usr/include/boost/mpl/lambda_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/void_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/na.hpp /usr/include/boost/mpl/bool.hpp
hoxUtil.o: /usr/include/boost/mpl/bool_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/na_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/ctps.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/lambda.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/ttp.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/arity.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/dtp.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/template_arity_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/preprocessor/params.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/preprocessor.hpp
hoxUtil.o: /usr/include/boost/preprocessor/comma_if.hpp
hoxUtil.o: /usr/include/boost/preprocessor/punctuation/comma_if.hpp
hoxUtil.o: /usr/include/boost/preprocessor/control/if.hpp
hoxUtil.o: /usr/include/boost/preprocessor/control/iif.hpp
hoxUtil.o: /usr/include/boost/preprocessor/logical/bool.hpp
hoxUtil.o: /usr/include/boost/preprocessor/facilities/empty.hpp
hoxUtil.o: /usr/include/boost/preprocessor/punctuation/comma.hpp
hoxUtil.o: /usr/include/boost/preprocessor/repeat.hpp
hoxUtil.o: /usr/include/boost/preprocessor/repetition/repeat.hpp
hoxUtil.o: /usr/include/boost/preprocessor/debug/error.hpp
hoxUtil.o: /usr/include/boost/preprocessor/detail/auto_rec.hpp
hoxUtil.o: /usr/include/boost/preprocessor/tuple/eat.hpp
hoxUtil.o: /usr/include/boost/preprocessor/inc.hpp
hoxUtil.o: /usr/include/boost/preprocessor/arithmetic/inc.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/preprocessor/enum.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/preprocessor/def_params_tail.hpp
hoxUtil.o: /usr/include/boost/mpl/limits/arity.hpp
hoxUtil.o: /usr/include/boost/preprocessor/logical/and.hpp
hoxUtil.o: /usr/include/boost/preprocessor/logical/bitand.hpp
hoxUtil.o: /usr/include/boost/preprocessor/identity.hpp
hoxUtil.o: /usr/include/boost/preprocessor/facilities/identity.hpp
hoxUtil.o: /usr/include/boost/preprocessor/empty.hpp
hoxUtil.o: /usr/include/boost/preprocessor/arithmetic/add.hpp
hoxUtil.o: /usr/include/boost/preprocessor/arithmetic/dec.hpp
hoxUtil.o: /usr/include/boost/preprocessor/control/while.hpp
hoxUtil.o: /usr/include/boost/preprocessor/list/fold_left.hpp
hoxUtil.o: /usr/include/boost/preprocessor/list/detail/fold_left.hpp
hoxUtil.o: /usr/include/boost/preprocessor/control/expr_iif.hpp
hoxUtil.o: /usr/include/boost/preprocessor/list/adt.hpp
hoxUtil.o: /usr/include/boost/preprocessor/detail/is_binary.hpp
hoxUtil.o: /usr/include/boost/preprocessor/detail/check.hpp
hoxUtil.o: /usr/include/boost/preprocessor/logical/compl.hpp
hoxUtil.o: /usr/include/boost/preprocessor/list/fold_right.hpp
hoxUtil.o: /usr/include/boost/preprocessor/list/detail/fold_right.hpp
hoxUtil.o: /usr/include/boost/preprocessor/list/reverse.hpp
hoxUtil.o: /usr/include/boost/preprocessor/control/detail/while.hpp
hoxUtil.o: /usr/include/boost/preprocessor/tuple/elem.hpp
hoxUtil.o: /usr/include/boost/preprocessor/arithmetic/sub.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/lambda_arity_param.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/overload_resolution.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/lambda_support.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/yes_no.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/arrays.hpp
hoxUtil.o: /usr/include/boost/preprocessor/tuple/to_list.hpp
hoxUtil.o: /usr/include/boost/preprocessor/list/for_each_i.hpp
hoxUtil.o: /usr/include/boost/preprocessor/repetition/for.hpp
hoxUtil.o: /usr/include/boost/preprocessor/repetition/detail/for.hpp
hoxUtil.o: /usr/include/boost/preprocessor/tuple/rem.hpp
hoxUtil.o: /usr/include/boost/mpl/identity.hpp
hoxUtil.o: /usr/include/boost/mpl/placeholders.hpp
hoxUtil.o: /usr/include/boost/mpl/arg.hpp /usr/include/boost/mpl/arg_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/na_assert.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/arity_spec.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/arg_typedef.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/use_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/compiler.hpp
hoxUtil.o: /usr/include/boost/preprocessor/stringize.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_convertible.hpp
hoxUtil.o: /usr/include/boost/type_traits/intrinsics.hpp
hoxUtil.o: /usr/include/boost/type_traits/config.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/yes_no_type.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_array.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/bool_trait_def.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/template_arity_spec.hpp
hoxUtil.o: /usr/include/boost/type_traits/integral_constant.hpp
hoxUtil.o: /usr/include/boost/mpl/integral_c.hpp
hoxUtil.o: /usr/include/boost/mpl/integral_c_fwd.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/bool_trait_undef.hpp
hoxUtil.o: /usr/include/boost/type_traits/add_reference.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_reference.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/type_trait_def.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/type_trait_undef.hpp
hoxUtil.o: /usr/include/boost/type_traits/ice.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/ice_or.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/ice_and.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/ice_not.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/ice_eq.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_arithmetic.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_integral.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_float.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_void.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_abstract.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_class.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_union.hpp
hoxUtil.o: /usr/include/boost/type_traits/remove_cv.hpp
hoxUtil.o: /usr/include/boost/type_traits/broken_compiler_spec.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/cv_traits_impl.hpp
hoxUtil.o: /usr/include/boost/iterator/detail/config_undef.hpp
hoxUtil.o: /usr/include/boost/iterator/iterator_facade.hpp
hoxUtil.o: /usr/include/boost/iterator/interoperable.hpp
hoxUtil.o: /usr/include/boost/mpl/or.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/nested_type_wknd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/iterator/iterator_traits.hpp
hoxUtil.o: /usr/include/boost/iterator/detail/facade_iterator_category.hpp
hoxUtil.o: /usr/include/boost/mpl/and.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/assert.hpp /usr/include/boost/mpl/not.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/pp_counter.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_same.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_const.hpp
hoxUtil.o: /usr/include/boost/detail/indirect_traits.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_function.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/false_result.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/is_function_ptr_helper.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_pointer.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_member_pointer.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_member_function_pointer.hpp
hoxUtil.o: /usr/include/boost/type_traits/detail/is_mem_fun_pointer_impl.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_volatile.hpp
hoxUtil.o: /usr/include/boost/type_traits/remove_reference.hpp
hoxUtil.o: /usr/include/boost/type_traits/remove_pointer.hpp
hoxUtil.o: /usr/include/boost/iterator/detail/enable_if.hpp
hoxUtil.o: /usr/include/boost/implicit_cast.hpp
hoxUtil.o: /usr/include/boost/type_traits/add_const.hpp
hoxUtil.o: /usr/include/boost/type_traits/add_pointer.hpp
hoxUtil.o: /usr/include/boost/type_traits/remove_const.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_pod.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_scalar.hpp
hoxUtil.o: /usr/include/boost/type_traits/is_enum.hpp
hoxUtil.o: /usr/include/boost/mpl/always.hpp /usr/include/boost/mpl/apply.hpp
hoxUtil.o: /usr/include/boost/mpl/apply_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/apply_wrap.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/has_apply.hpp
hoxUtil.o: /usr/include/boost/mpl/has_xxx.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/type_wrapper.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/has_xxx.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/msvc_typename.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/has_apply.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/msvc_never_true.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/lambda.hpp /usr/include/boost/mpl/bind.hpp
hoxUtil.o: /usr/include/boost/mpl/bind_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/bind.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/next.hpp
hoxUtil.o: /usr/include/boost/mpl/next_prior.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/common_name_wknd.hpp
hoxUtil.o: /usr/include/boost/mpl/protect.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/lambda_no_ctps.hpp
hoxUtil.o: /usr/include/boost/mpl/is_placeholder.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/template_arity.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/has_rebind.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/iterator/detail/minimum_category.hpp
hoxUtil.o: /usr/include/boost/token_functions.hpp hoxUtil.h hoxTypes.h
hoxUtil.o: /usr/include/boost/shared_ptr.hpp
hoxUtil.o: /usr/include/boost/config/no_tr1/memory.hpp
hoxUtil.o: /usr/include/boost/checked_delete.hpp
hoxUtil.o: /usr/include/boost/throw_exception.hpp
hoxUtil.o: /usr/include/boost/detail/shared_count.hpp
hoxUtil.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxUtil.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxUtil.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxUtil.o: /usr/include/boost/detail/sp_typeinfo.hpp
hoxUtil.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxUtil.o: /usr/include/boost/detail/sp_convertible.hpp
hoxUtil.o: /usr/include/boost/detail/spinlock_pool.hpp
hoxUtil.o: /usr/include/boost/detail/spinlock.hpp
hoxUtil.o: /usr/include/boost/detail/spinlock_pt.hpp /usr/include/pthread.h
hoxUtil.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxUtil.o: /usr/include/bits/byteswap.h /usr/include/sched.h
hoxUtil.o: /usr/include/time.h /usr/include/bits/time.h
hoxUtil.o: /usr/include/xlocale.h /usr/include/bits/sched.h
hoxUtil.o: /usr/include/signal.h /usr/include/bits/sigset.h
hoxUtil.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
hoxUtil.o: /usr/include/boost/memory_order.hpp hoxEnums.h
hoxTypes.o: /usr/include/boost/tokenizer.hpp
hoxTypes.o: /usr/include/boost/token_iterator.hpp
hoxTypes.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxTypes.o: /usr/include/features.h /usr/include/bits/predefs.h
hoxTypes.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxTypes.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxTypes.o: /usr/include/boost/iterator/iterator_adaptor.hpp
hoxTypes.o: /usr/include/boost/static_assert.hpp
hoxTypes.o: /usr/include/boost/config.hpp /usr/include/boost/config/user.hpp
hoxTypes.o: /usr/include/boost/config/select_compiler_config.hpp
hoxTypes.o: /usr/include/boost/config/compiler/gcc.hpp
hoxTypes.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxTypes.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxTypes.o: /usr/include/boost/config/select_platform_config.hpp
hoxTypes.o: /usr/include/boost/config/posix_features.hpp
hoxTypes.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
hoxTypes.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
hoxTypes.o: /usr/include/bits/confname.h /usr/include/getopt.h
hoxTypes.o: /usr/include/boost/config/suffix.hpp
hoxTypes.o: /usr/include/boost/detail/workaround.hpp
hoxTypes.o: /usr/include/boost/config.hpp /usr/include/boost/iterator.hpp
hoxTypes.o: /usr/include/boost/detail/iterator.hpp
hoxTypes.o: /usr/include/boost/iterator/iterator_categories.hpp
hoxTypes.o: /usr/include/boost/iterator/detail/config_def.hpp
hoxTypes.o: /usr/include/boost/mpl/eval_if.hpp /usr/include/boost/mpl/if.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/value_wknd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/static_cast.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/workaround.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/integral.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/msvc.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/eti.hpp
hoxTypes.o: /usr/include/boost/mpl/int.hpp /usr/include/boost/mpl/int_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/adl_barrier.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/adl.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/intel.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/gcc.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/nttp_decl.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/nttp.hpp
hoxTypes.o: /usr/include/boost/preprocessor/cat.hpp
hoxTypes.o: /usr/include/boost/preprocessor/config/config.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/integral_wrapper.hpp
hoxTypes.o: /usr/include/boost/mpl/integral_c_tag.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/static_constant.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/na_spec.hpp
hoxTypes.o: /usr/include/boost/mpl/lambda_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/void_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/na.hpp
hoxTypes.o: /usr/include/boost/mpl/bool.hpp
hoxTypes.o: /usr/include/boost/mpl/bool_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/na_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/ctps.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/lambda.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/ttp.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/arity.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/dtp.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/template_arity_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/preprocessor/params.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/preprocessor.hpp
hoxTypes.o: /usr/include/boost/preprocessor/comma_if.hpp
hoxTypes.o: /usr/include/boost/preprocessor/punctuation/comma_if.hpp
hoxTypes.o: /usr/include/boost/preprocessor/control/if.hpp
hoxTypes.o: /usr/include/boost/preprocessor/control/iif.hpp
hoxTypes.o: /usr/include/boost/preprocessor/logical/bool.hpp
hoxTypes.o: /usr/include/boost/preprocessor/facilities/empty.hpp
hoxTypes.o: /usr/include/boost/preprocessor/punctuation/comma.hpp
hoxTypes.o: /usr/include/boost/preprocessor/repeat.hpp
hoxTypes.o: /usr/include/boost/preprocessor/repetition/repeat.hpp
hoxTypes.o: /usr/include/boost/preprocessor/debug/error.hpp
hoxTypes.o: /usr/include/boost/preprocessor/detail/auto_rec.hpp
hoxTypes.o: /usr/include/boost/preprocessor/tuple/eat.hpp
hoxTypes.o: /usr/include/boost/preprocessor/inc.hpp
hoxTypes.o: /usr/include/boost/preprocessor/arithmetic/inc.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/preprocessor/enum.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/preprocessor/def_params_tail.hpp
hoxTypes.o: /usr/include/boost/mpl/limits/arity.hpp
hoxTypes.o: /usr/include/boost/preprocessor/logical/and.hpp
hoxTypes.o: /usr/include/boost/preprocessor/logical/bitand.hpp
hoxTypes.o: /usr/include/boost/preprocessor/identity.hpp
hoxTypes.o: /usr/include/boost/preprocessor/facilities/identity.hpp
hoxTypes.o: /usr/include/boost/preprocessor/empty.hpp
hoxTypes.o: /usr/include/boost/preprocessor/arithmetic/add.hpp
hoxTypes.o: /usr/include/boost/preprocessor/arithmetic/dec.hpp
hoxTypes.o: /usr/include/boost/preprocessor/control/while.hpp
hoxTypes.o: /usr/include/boost/preprocessor/list/fold_left.hpp
hoxTypes.o: /usr/include/boost/preprocessor/list/detail/fold_left.hpp
hoxTypes.o: /usr/include/boost/preprocessor/control/expr_iif.hpp
hoxTypes.o: /usr/include/boost/preprocessor/list/adt.hpp
hoxTypes.o: /usr/include/boost/preprocessor/detail/is_binary.hpp
hoxTypes.o: /usr/include/boost/preprocessor/detail/check.hpp
hoxTypes.o: /usr/include/boost/preprocessor/logical/compl.hpp
hoxTypes.o: /usr/include/boost/preprocessor/list/fold_right.hpp
hoxTypes.o: /usr/include/boost/preprocessor/list/detail/fold_right.hpp
hoxTypes.o: /usr/include/boost/preprocessor/list/reverse.hpp
hoxTypes.o: /usr/include/boost/preprocessor/control/detail/while.hpp
hoxTypes.o: /usr/include/boost/preprocessor/tuple/elem.hpp
hoxTypes.o: /usr/include/boost/preprocessor/arithmetic/sub.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/lambda_arity_param.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/overload_resolution.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/lambda_support.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/yes_no.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/arrays.hpp
hoxTypes.o: /usr/include/boost/preprocessor/tuple/to_list.hpp
hoxTypes.o: /usr/include/boost/preprocessor/list/for_each_i.hpp
hoxTypes.o: /usr/include/boost/preprocessor/repetition/for.hpp
hoxTypes.o: /usr/include/boost/preprocessor/repetition/detail/for.hpp
hoxTypes.o: /usr/include/boost/preprocessor/tuple/rem.hpp
hoxTypes.o: /usr/include/boost/mpl/identity.hpp
hoxTypes.o: /usr/include/boost/mpl/placeholders.hpp
hoxTypes.o: /usr/include/boost/mpl/arg.hpp /usr/include/boost/mpl/arg_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/na_assert.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/arity_spec.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/arg_typedef.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/use_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/compiler.hpp
hoxTypes.o: /usr/include/boost/preprocessor/stringize.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_convertible.hpp
hoxTypes.o: /usr/include/boost/type_traits/intrinsics.hpp
hoxTypes.o: /usr/include/boost/type_traits/config.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/yes_no_type.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_array.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/bool_trait_def.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/template_arity_spec.hpp
hoxTypes.o: /usr/include/boost/type_traits/integral_constant.hpp
hoxTypes.o: /usr/include/boost/mpl/integral_c.hpp
hoxTypes.o: /usr/include/boost/mpl/integral_c_fwd.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/bool_trait_undef.hpp
hoxTypes.o: /usr/include/boost/type_traits/add_reference.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_reference.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/type_trait_def.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/type_trait_undef.hpp
hoxTypes.o: /usr/include/boost/type_traits/ice.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/ice_or.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/ice_and.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/ice_not.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/ice_eq.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_arithmetic.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_integral.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_float.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_void.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_abstract.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_class.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_union.hpp
hoxTypes.o: /usr/include/boost/type_traits/remove_cv.hpp
hoxTypes.o: /usr/include/boost/type_traits/broken_compiler_spec.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/cv_traits_impl.hpp
hoxTypes.o: /usr/include/boost/iterator/detail/config_undef.hpp
hoxTypes.o: /usr/include/boost/iterator/iterator_facade.hpp
hoxTypes.o: /usr/include/boost/iterator/interoperable.hpp
hoxTypes.o: /usr/include/boost/mpl/or.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/nested_type_wknd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/iterator/iterator_traits.hpp
hoxTypes.o: /usr/include/boost/iterator/detail/facade_iterator_category.hpp
hoxTypes.o: /usr/include/boost/mpl/and.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/assert.hpp /usr/include/boost/mpl/not.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/pp_counter.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_same.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_const.hpp
hoxTypes.o: /usr/include/boost/detail/indirect_traits.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_function.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/false_result.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/is_function_ptr_helper.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_pointer.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_member_pointer.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_member_function_pointer.hpp
hoxTypes.o: /usr/include/boost/type_traits/detail/is_mem_fun_pointer_impl.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_volatile.hpp
hoxTypes.o: /usr/include/boost/type_traits/remove_reference.hpp
hoxTypes.o: /usr/include/boost/type_traits/remove_pointer.hpp
hoxTypes.o: /usr/include/boost/iterator/detail/enable_if.hpp
hoxTypes.o: /usr/include/boost/implicit_cast.hpp
hoxTypes.o: /usr/include/boost/type_traits/add_const.hpp
hoxTypes.o: /usr/include/boost/type_traits/add_pointer.hpp
hoxTypes.o: /usr/include/boost/type_traits/remove_const.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_pod.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_scalar.hpp
hoxTypes.o: /usr/include/boost/type_traits/is_enum.hpp
hoxTypes.o: /usr/include/boost/mpl/always.hpp
hoxTypes.o: /usr/include/boost/mpl/apply.hpp
hoxTypes.o: /usr/include/boost/mpl/apply_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/apply_wrap.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/has_apply.hpp
hoxTypes.o: /usr/include/boost/mpl/has_xxx.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/type_wrapper.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/has_xxx.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/msvc_typename.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/has_apply.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/msvc_never_true.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/lambda.hpp /usr/include/boost/mpl/bind.hpp
hoxTypes.o: /usr/include/boost/mpl/bind_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/bind.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/next.hpp
hoxTypes.o: /usr/include/boost/mpl/next_prior.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/common_name_wknd.hpp
hoxTypes.o: /usr/include/boost/mpl/protect.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/lambda_no_ctps.hpp
hoxTypes.o: /usr/include/boost/mpl/is_placeholder.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/template_arity.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/has_rebind.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/iterator/detail/minimum_category.hpp
hoxTypes.o: /usr/include/boost/token_functions.hpp hoxTypes.h
hoxTypes.o: /usr/include/boost/shared_ptr.hpp
hoxTypes.o: /usr/include/boost/config/no_tr1/memory.hpp
hoxTypes.o: /usr/include/boost/checked_delete.hpp
hoxTypes.o: /usr/include/boost/throw_exception.hpp
hoxTypes.o: /usr/include/boost/detail/shared_count.hpp
hoxTypes.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxTypes.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxTypes.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxTypes.o: /usr/include/boost/detail/sp_typeinfo.hpp
hoxTypes.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxTypes.o: /usr/include/boost/detail/sp_convertible.hpp
hoxTypes.o: /usr/include/boost/detail/spinlock_pool.hpp
hoxTypes.o: /usr/include/boost/detail/spinlock.hpp
hoxTypes.o: /usr/include/boost/detail/spinlock_pt.hpp /usr/include/pthread.h
hoxTypes.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxTypes.o: /usr/include/bits/byteswap.h /usr/include/sched.h
hoxTypes.o: /usr/include/time.h /usr/include/bits/time.h
hoxTypes.o: /usr/include/xlocale.h /usr/include/bits/sched.h
hoxTypes.o: /usr/include/signal.h /usr/include/bits/sigset.h
hoxTypes.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
hoxTypes.o: /usr/include/boost/memory_order.hpp hoxEnums.h hoxUtil.h
hoxDBAPI.o: hoxDBAPI.h hoxEnums.h hoxLog.h hoxUtil.h hoxTypes.h
hoxDBAPI.o: /usr/include/boost/shared_ptr.hpp /usr/include/boost/config.hpp
hoxDBAPI.o: /usr/include/boost/config/no_tr1/memory.hpp
hoxDBAPI.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxDBAPI.o: /usr/include/features.h /usr/include/bits/predefs.h
hoxDBAPI.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxDBAPI.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxDBAPI.o: /usr/include/boost/checked_delete.hpp
hoxDBAPI.o: /usr/include/boost/throw_exception.hpp
hoxDBAPI.o: /usr/include/boost/detail/workaround.hpp
hoxDBAPI.o: /usr/include/boost/detail/shared_count.hpp
hoxDBAPI.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxDBAPI.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxDBAPI.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxDBAPI.o: /usr/include/boost/detail/sp_typeinfo.hpp
hoxDBAPI.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxDBAPI.o: /usr/include/boost/detail/sp_convertible.hpp
hoxDBAPI.o: /usr/include/boost/detail/spinlock_pool.hpp
hoxDBAPI.o: /usr/include/boost/detail/spinlock.hpp
hoxDBAPI.o: /usr/include/boost/detail/spinlock_pt.hpp /usr/include/pthread.h
hoxDBAPI.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxDBAPI.o: /usr/include/bits/byteswap.h /usr/include/sched.h
hoxDBAPI.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
hoxDBAPI.o: /usr/include/time.h /usr/include/bits/time.h
hoxDBAPI.o: /usr/include/xlocale.h /usr/include/bits/sched.h
hoxDBAPI.o: /usr/include/signal.h /usr/include/bits/sigset.h
hoxDBAPI.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/setjmp.h
hoxDBAPI.o: /usr/include/boost/memory_order.hpp /usr/include/sqlite3.h
main.o: /usr/include/arpa/inet.h /usr/include/features.h
main.o: /usr/include/bits/predefs.h /usr/include/sys/cdefs.h
main.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
main.o: /usr/include/gnu/stubs-32.h /usr/include/netinet/in.h
main.o: /usr/include/stdint.h /usr/include/bits/wchar.h
main.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
main.o: /usr/include/sys/types.h /usr/include/bits/types.h
main.o: /usr/include/bits/typesizes.h /usr/include/time.h
main.o: /usr/include/bits/time.h /usr/include/xlocale.h /usr/include/endian.h
main.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
main.o: /usr/include/sys/select.h /usr/include/bits/select.h
main.o: /usr/include/bits/sigset.h /usr/include/sys/sysmacros.h
main.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
main.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
main.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
main.o: /usr/include/bits/in.h /usr/include/netdb.h /usr/include/rpc/netdb.h
main.o: /usr/include/bits/netdb.h /usr/include/fcntl.h
main.o: /usr/include/bits/fcntl.h /usr/include/unistd.h
main.o: /usr/include/bits/posix_opt.h /usr/include/bits/confname.h
main.o: /usr/include/getopt.h /usr/include/pthread.h /usr/include/sched.h
main.o: /usr/include/bits/sched.h /usr/include/signal.h
main.o: /usr/include/bits/setjmp.h /usr/include/errno.h
main.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
main.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
main.o: /usr/include/asm-generic/errno-base.h /usr/include/string.h
main.o: /usr/include/stdio.h /usr/include/libio.h /usr/include/_G_config.h
main.o: /usr/include/wchar.h /usr/include/bits/stdio_lim.h
main.o: /usr/include/bits/sys_errlist.h hoxLog.h hoxTypes.h
main.o: /usr/include/boost/shared_ptr.hpp /usr/include/boost/config.hpp
main.o: /usr/include/boost/config/no_tr1/memory.hpp
main.o: /usr/include/boost/assert.hpp /usr/include/assert.h
main.o: /usr/include/boost/checked_delete.hpp
main.o: /usr/include/boost/throw_exception.hpp
main.o: /usr/include/boost/detail/workaround.hpp
main.o: /usr/include/boost/detail/shared_count.hpp
main.o: /usr/include/boost/detail/bad_weak_ptr.hpp
main.o: /usr/include/boost/detail/sp_counted_base.hpp
main.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
main.o: /usr/include/boost/detail/sp_typeinfo.hpp
main.o: /usr/include/boost/detail/sp_counted_impl.hpp
main.o: /usr/include/boost/detail/sp_convertible.hpp
main.o: /usr/include/boost/detail/spinlock_pool.hpp
main.o: /usr/include/boost/detail/spinlock.hpp
main.o: /usr/include/boost/detail/spinlock_pt.hpp
main.o: /usr/include/boost/memory_order.hpp hoxEnums.h hoxSocketAPI.h
main.o: hoxDBAPI.h hoxExcept.h hoxUtil.h

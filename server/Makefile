# Your project's name.
PROGRAM = hoxserver

# Libraries specific flags.
ST_CXXFLAGS    = -I../include
ST_LDLIBS      = -L../lib -lst -lconfig++
#BOOST_CXXFLAGS = -I/usr/include/boost
BOOST_CXXFLAGS =

# Common flags
CXX         = g++
CXXFLAGS    = $(ST_CXXFLAGS) $(BOOST_CXXFLAGS) -Wall -Werror -Wno-char-subscripts
LDLIBS      = $(ST_LDLIBS)
LDFLAGS     = -rdynamic
#DEBUGFLAGS  = -g
DEBUGFLAGS  =

# Define our sources and object files
SOURCES := \
	hoxLog.cpp \
	hoxDebug.cpp \
	hoxExcept.cpp \
	hoxTypes.cpp \
	hoxSocketAPI.cpp \
	hoxSessionMgr.cpp \
	hoxSession.cpp \
	hoxPlayer.cpp \
	hoxTable.cpp \
	hoxMove.cpp \
	hoxReferee.cpp \
	hoxUtil.cpp \
	hoxDbClient.cpp \
	hoxFileMgr.cpp \
	session.cpp \
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
	zip $(PROGRAM) $(SOURCES) *.h Makefile *.py *.sh


# DO NOT DELETE THIS LINE -- make depend depends on it.

hoxLog.o: /usr/include/string.h /usr/include/features.h
hoxLog.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxLog.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxLog.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
hoxLog.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
hoxLog.o: /usr/include/bits/confname.h /usr/include/getopt.h
hoxLog.o: /usr/include/errno.h /usr/include/bits/errno.h
hoxLog.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
hoxLog.o: /usr/include/asm-generic/errno.h
hoxLog.o: /usr/include/asm-generic/errno-base.h ../include/st.h
hoxLog.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
hoxLog.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
hoxLog.o: /usr/include/sys/select.h /usr/include/bits/select.h
hoxLog.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
hoxLog.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
hoxLog.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
hoxLog.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
hoxLog.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
hoxLog.o: /usr/include/asm/sockios.h /usr/include/poll.h
hoxLog.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxLog.h main.h
hoxLog.o: hoxDbClient.h hoxTypes.h /usr/include/boost/shared_ptr.hpp
hoxLog.o: /usr/include/boost/config.hpp /usr/include/boost/config/user.hpp
hoxLog.o: /usr/include/boost/config/select_compiler_config.hpp
hoxLog.o: /usr/include/boost/config/compiler/gcc.hpp
hoxLog.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxLog.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxLog.o: /usr/include/boost/config/select_platform_config.hpp
hoxLog.o: /usr/include/boost/config/posix_features.hpp
hoxLog.o: /usr/include/boost/config/suffix.hpp /usr/include/boost/assert.hpp
hoxLog.o: /usr/include/assert.h /usr/include/boost/checked_delete.hpp
hoxLog.o: /usr/include/boost/throw_exception.hpp
hoxLog.o: /usr/include/boost/config.hpp
hoxLog.o: /usr/include/boost/detail/shared_count.hpp
hoxLog.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxLog.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxLog.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxLog.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxLog.o: /usr/include/boost/detail/workaround.hpp hoxEnums.h hoxDebug.h
hoxDebug.o: hoxDebug.h hoxLog.h
hoxExcept.o: hoxExcept.h hoxEnums.h
hoxTypes.o: /usr/include/boost/tokenizer.hpp
hoxTypes.o: /usr/include/boost/token_iterator.hpp
hoxTypes.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxTypes.o: /usr/include/features.h /usr/include/sys/cdefs.h
hoxTypes.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
hoxTypes.o: /usr/include/gnu/stubs-32.h
hoxTypes.o: /usr/include/boost/iterator/iterator_adaptor.hpp
hoxTypes.o: /usr/include/boost/static_assert.hpp
hoxTypes.o: /usr/include/boost/config.hpp
hoxTypes.o: /usr/include/boost/detail/workaround.hpp
hoxTypes.o: /usr/include/boost/iterator.hpp
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
hoxTypes.o: /usr/include/boost/mpl/aux_/lambda_arity_param.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/template_arity_fwd.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/arity.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/dtp.hpp
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
hoxTypes.o: /usr/include/boost/mpl/aux_/config/overload_resolution.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/lambda_support.hpp
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
hoxTypes.o: /usr/include/boost/type_traits/detail/yes_no_type.hpp
hoxTypes.o: /usr/include/boost/type_traits/config.hpp
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
hoxTypes.o: /usr/include/boost/type_traits/intrinsics.hpp
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
hoxTypes.o: /usr/include/boost/mpl/not.hpp
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
hoxTypes.o: /usr/include/boost/mpl/aux_/yes_no.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/config/arrays.hpp
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
hoxTypes.o: /usr/include/boost/mpl/aux_/full_lambda.hpp
hoxTypes.o: /usr/include/boost/mpl/quote.hpp /usr/include/boost/mpl/void.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/has_type.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/template_arity.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxTypes.o: /usr/include/boost/iterator/detail/minimum_category.hpp
hoxTypes.o: /usr/include/boost/token_functions.hpp hoxTypes.h
hoxTypes.o: /usr/include/boost/shared_ptr.hpp /usr/include/boost/config.hpp
hoxTypes.o: /usr/include/boost/config/user.hpp
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
hoxTypes.o: /usr/include/boost/checked_delete.hpp
hoxTypes.o: /usr/include/boost/throw_exception.hpp
hoxTypes.o: /usr/include/boost/detail/shared_count.hpp
hoxTypes.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxTypes.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxTypes.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxTypes.o: /usr/include/boost/detail/sp_counted_impl.hpp ../include/st.h
hoxTypes.o: /usr/include/sys/types.h /usr/include/time.h
hoxTypes.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxTypes.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
hoxTypes.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
hoxTypes.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
hoxTypes.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
hoxTypes.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
hoxTypes.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
hoxTypes.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
hoxTypes.o: /usr/include/errno.h /usr/include/bits/errno.h
hoxTypes.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
hoxTypes.o: /usr/include/asm-generic/errno.h
hoxTypes.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxTypes.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxEnums.h
hoxTypes.o: hoxDebug.h hoxUtil.h hoxTable.h hoxPlayer.h
hoxTypes.o: /usr/include/boost/enable_shared_from_this.hpp
hoxTypes.o: /usr/include/boost/weak_ptr.hpp hoxSocketAPI.h
hoxSocketAPI.o: hoxSocketAPI.h ../include/st.h /usr/include/unistd.h
hoxSocketAPI.o: /usr/include/features.h /usr/include/sys/cdefs.h
hoxSocketAPI.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
hoxSocketAPI.o: /usr/include/gnu/stubs-32.h /usr/include/bits/posix_opt.h
hoxSocketAPI.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
hoxSocketAPI.o: /usr/include/bits/confname.h /usr/include/getopt.h
hoxSocketAPI.o: /usr/include/sys/types.h /usr/include/time.h
hoxSocketAPI.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxSocketAPI.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
hoxSocketAPI.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
hoxSocketAPI.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
hoxSocketAPI.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
hoxSocketAPI.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
hoxSocketAPI.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
hoxSocketAPI.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
hoxSocketAPI.o: /usr/include/errno.h /usr/include/bits/errno.h
hoxSocketAPI.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
hoxSocketAPI.o: /usr/include/asm-generic/errno.h
hoxSocketAPI.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxSocketAPI.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxEnums.h
hoxSocketAPI.o: hoxLog.h
hoxSessionMgr.o: hoxSessionMgr.h hoxSession.h
hoxSessionMgr.o: /usr/include/boost/enable_shared_from_this.hpp
hoxSessionMgr.o: /usr/include/boost/weak_ptr.hpp
hoxSessionMgr.o: /usr/include/boost/detail/shared_count.hpp
hoxSessionMgr.o: /usr/include/boost/config.hpp
hoxSessionMgr.o: /usr/include/boost/checked_delete.hpp
hoxSessionMgr.o: /usr/include/boost/throw_exception.hpp
hoxSessionMgr.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxSessionMgr.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxSessionMgr.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxSessionMgr.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxSessionMgr.o: /usr/include/boost/shared_ptr.hpp
hoxSessionMgr.o: /usr/include/boost/config.hpp
hoxSessionMgr.o: /usr/include/boost/config/user.hpp
hoxSessionMgr.o: /usr/include/boost/config/select_compiler_config.hpp
hoxSessionMgr.o: /usr/include/boost/config/compiler/gcc.hpp
hoxSessionMgr.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxSessionMgr.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxSessionMgr.o: /usr/include/boost/config/select_platform_config.hpp
hoxSessionMgr.o: /usr/include/boost/config/posix_features.hpp
hoxSessionMgr.o: /usr/include/unistd.h /usr/include/features.h
hoxSessionMgr.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxSessionMgr.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxSessionMgr.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
hoxSessionMgr.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
hoxSessionMgr.o: /usr/include/getopt.h /usr/include/boost/config/suffix.hpp
hoxSessionMgr.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxSessionMgr.o: /usr/include/boost/detail/workaround.hpp ../include/st.h
hoxSessionMgr.o: /usr/include/sys/types.h /usr/include/time.h
hoxSessionMgr.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxSessionMgr.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
hoxSessionMgr.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
hoxSessionMgr.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
hoxSessionMgr.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
hoxSessionMgr.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
hoxSessionMgr.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
hoxSessionMgr.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
hoxSessionMgr.o: /usr/include/errno.h /usr/include/bits/errno.h
hoxSessionMgr.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
hoxSessionMgr.o: /usr/include/asm-generic/errno.h
hoxSessionMgr.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxSessionMgr.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxTypes.h
hoxSessionMgr.o: hoxEnums.h hoxDebug.h hoxPlayer.h hoxTable.h hoxLog.h
hoxSessionMgr.o: hoxUtil.h
hoxSession.o: hoxSession.h /usr/include/boost/enable_shared_from_this.hpp
hoxSession.o: /usr/include/boost/weak_ptr.hpp
hoxSession.o: /usr/include/boost/detail/shared_count.hpp
hoxSession.o: /usr/include/boost/config.hpp
hoxSession.o: /usr/include/boost/checked_delete.hpp
hoxSession.o: /usr/include/boost/throw_exception.hpp
hoxSession.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxSession.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxSession.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxSession.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxSession.o: /usr/include/boost/shared_ptr.hpp /usr/include/boost/config.hpp
hoxSession.o: /usr/include/boost/config/user.hpp
hoxSession.o: /usr/include/boost/config/select_compiler_config.hpp
hoxSession.o: /usr/include/boost/config/compiler/gcc.hpp
hoxSession.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxSession.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxSession.o: /usr/include/boost/config/select_platform_config.hpp
hoxSession.o: /usr/include/boost/config/posix_features.hpp
hoxSession.o: /usr/include/unistd.h /usr/include/features.h
hoxSession.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxSession.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxSession.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
hoxSession.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
hoxSession.o: /usr/include/getopt.h /usr/include/boost/config/suffix.hpp
hoxSession.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxSession.o: /usr/include/boost/detail/workaround.hpp ../include/st.h
hoxSession.o: /usr/include/sys/types.h /usr/include/time.h
hoxSession.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxSession.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
hoxSession.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
hoxSession.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
hoxSession.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
hoxSession.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
hoxSession.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
hoxSession.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
hoxSession.o: /usr/include/errno.h /usr/include/bits/errno.h
hoxSession.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
hoxSession.o: /usr/include/asm-generic/errno.h
hoxSession.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxSession.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxTypes.h
hoxSession.o: hoxEnums.h hoxDebug.h hoxSessionMgr.h hoxPlayer.h hoxTable.h
hoxSession.o: hoxLog.h hoxSocketAPI.h hoxExcept.h hoxUtil.h hoxFileMgr.h
hoxPlayer.o: hoxPlayer.h /usr/include/boost/enable_shared_from_this.hpp
hoxPlayer.o: /usr/include/boost/weak_ptr.hpp
hoxPlayer.o: /usr/include/boost/detail/shared_count.hpp
hoxPlayer.o: /usr/include/boost/config.hpp
hoxPlayer.o: /usr/include/boost/checked_delete.hpp
hoxPlayer.o: /usr/include/boost/throw_exception.hpp
hoxPlayer.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxPlayer.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxPlayer.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxPlayer.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxPlayer.o: /usr/include/boost/shared_ptr.hpp /usr/include/boost/config.hpp
hoxPlayer.o: /usr/include/boost/config/user.hpp
hoxPlayer.o: /usr/include/boost/config/select_compiler_config.hpp
hoxPlayer.o: /usr/include/boost/config/compiler/gcc.hpp
hoxPlayer.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxPlayer.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxPlayer.o: /usr/include/boost/config/select_platform_config.hpp
hoxPlayer.o: /usr/include/boost/config/posix_features.hpp
hoxPlayer.o: /usr/include/unistd.h /usr/include/features.h
hoxPlayer.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxPlayer.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxPlayer.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
hoxPlayer.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
hoxPlayer.o: /usr/include/getopt.h /usr/include/boost/config/suffix.hpp
hoxPlayer.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxPlayer.o: /usr/include/boost/detail/workaround.hpp hoxTypes.h
hoxPlayer.o: ../include/st.h /usr/include/sys/types.h /usr/include/time.h
hoxPlayer.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxPlayer.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
hoxPlayer.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
hoxPlayer.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
hoxPlayer.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
hoxPlayer.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
hoxPlayer.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
hoxPlayer.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
hoxPlayer.o: /usr/include/errno.h /usr/include/bits/errno.h
hoxPlayer.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
hoxPlayer.o: /usr/include/asm-generic/errno.h
hoxPlayer.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxPlayer.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxEnums.h
hoxPlayer.o: hoxDebug.h hoxTable.h hoxSession.h hoxLog.h hoxExcept.h
hoxPlayer.o: hoxUtil.h
hoxTable.o: ../include/st.h /usr/include/unistd.h /usr/include/features.h
hoxTable.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxTable.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxTable.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
hoxTable.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
hoxTable.o: /usr/include/getopt.h /usr/include/sys/types.h
hoxTable.o: /usr/include/time.h /usr/include/endian.h
hoxTable.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
hoxTable.o: /usr/include/sys/select.h /usr/include/bits/select.h
hoxTable.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
hoxTable.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
hoxTable.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
hoxTable.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
hoxTable.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
hoxTable.o: /usr/include/asm/sockios.h /usr/include/errno.h
hoxTable.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
hoxTable.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
hoxTable.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxTable.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxTable.h
hoxTable.o: hoxPlayer.h /usr/include/boost/enable_shared_from_this.hpp
hoxTable.o: /usr/include/boost/weak_ptr.hpp
hoxTable.o: /usr/include/boost/detail/shared_count.hpp
hoxTable.o: /usr/include/boost/config.hpp
hoxTable.o: /usr/include/boost/checked_delete.hpp
hoxTable.o: /usr/include/boost/throw_exception.hpp
hoxTable.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxTable.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxTable.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxTable.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxTable.o: /usr/include/boost/shared_ptr.hpp /usr/include/boost/config.hpp
hoxTable.o: /usr/include/boost/config/user.hpp
hoxTable.o: /usr/include/boost/config/select_compiler_config.hpp
hoxTable.o: /usr/include/boost/config/compiler/gcc.hpp
hoxTable.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxTable.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxTable.o: /usr/include/boost/config/select_platform_config.hpp
hoxTable.o: /usr/include/boost/config/posix_features.hpp
hoxTable.o: /usr/include/boost/config/suffix.hpp
hoxTable.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxTable.o: /usr/include/boost/detail/workaround.hpp hoxTypes.h hoxEnums.h
hoxTable.o: hoxDebug.h hoxUtil.h hoxLog.h hoxReferee.h hoxMove.h
hoxTable.o: hoxDbClient.h
hoxMove.o: hoxMove.h hoxEnums.h
hoxReferee.o: hoxReferee.h hoxMove.h hoxEnums.h hoxLog.h hoxDebug.h
hoxUtil.o: /usr/include/boost/tokenizer.hpp
hoxUtil.o: /usr/include/boost/token_iterator.hpp
hoxUtil.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxUtil.o: /usr/include/features.h /usr/include/sys/cdefs.h
hoxUtil.o: /usr/include/bits/wordsize.h /usr/include/gnu/stubs.h
hoxUtil.o: /usr/include/gnu/stubs-32.h
hoxUtil.o: /usr/include/boost/iterator/iterator_adaptor.hpp
hoxUtil.o: /usr/include/boost/static_assert.hpp /usr/include/boost/config.hpp
hoxUtil.o: /usr/include/boost/detail/workaround.hpp
hoxUtil.o: /usr/include/boost/iterator.hpp
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
hoxUtil.o: /usr/include/boost/mpl/aux_/lambda_arity_param.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/template_arity_fwd.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/arity.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/dtp.hpp
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
hoxUtil.o: /usr/include/boost/mpl/aux_/config/overload_resolution.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/lambda_support.hpp
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
hoxUtil.o: /usr/include/boost/type_traits/detail/yes_no_type.hpp
hoxUtil.o: /usr/include/boost/type_traits/config.hpp
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
hoxUtil.o: /usr/include/boost/type_traits/intrinsics.hpp
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
hoxUtil.o: /usr/include/boost/mpl/not.hpp
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
hoxUtil.o: /usr/include/boost/mpl/aux_/yes_no.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/config/arrays.hpp
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
hoxUtil.o: /usr/include/boost/mpl/aux_/full_lambda.hpp
hoxUtil.o: /usr/include/boost/mpl/quote.hpp /usr/include/boost/mpl/void.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/has_type.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/template_arity.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxUtil.o: /usr/include/boost/iterator/detail/minimum_category.hpp
hoxUtil.o: /usr/include/boost/token_functions.hpp hoxUtil.h hoxTypes.h
hoxUtil.o: /usr/include/boost/shared_ptr.hpp /usr/include/boost/config.hpp
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
hoxUtil.o: /usr/include/boost/checked_delete.hpp
hoxUtil.o: /usr/include/boost/throw_exception.hpp
hoxUtil.o: /usr/include/boost/detail/shared_count.hpp
hoxUtil.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxUtil.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxUtil.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxUtil.o: /usr/include/boost/detail/sp_counted_impl.hpp ../include/st.h
hoxUtil.o: /usr/include/sys/types.h /usr/include/time.h /usr/include/endian.h
hoxUtil.o: /usr/include/bits/endian.h /usr/include/bits/byteswap.h
hoxUtil.o: /usr/include/sys/select.h /usr/include/bits/select.h
hoxUtil.o: /usr/include/bits/sigset.h /usr/include/bits/time.h
hoxUtil.o: /usr/include/sys/sysmacros.h /usr/include/bits/pthreadtypes.h
hoxUtil.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
hoxUtil.o: /usr/include/bits/uio.h /usr/include/bits/socket.h
hoxUtil.o: /usr/include/bits/sockaddr.h /usr/include/asm/socket.h
hoxUtil.o: /usr/include/asm/sockios.h /usr/include/errno.h
hoxUtil.o: /usr/include/bits/errno.h /usr/include/linux/errno.h
hoxUtil.o: /usr/include/asm/errno.h /usr/include/asm-generic/errno.h
hoxUtil.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxUtil.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxEnums.h
hoxUtil.o: hoxDebug.h hoxLog.h hoxSocketAPI.h
hoxDbClient.o: hoxDbClient.h hoxTypes.h /usr/include/boost/shared_ptr.hpp
hoxDbClient.o: /usr/include/boost/config.hpp
hoxDbClient.o: /usr/include/boost/config/user.hpp
hoxDbClient.o: /usr/include/boost/config/select_compiler_config.hpp
hoxDbClient.o: /usr/include/boost/config/compiler/gcc.hpp
hoxDbClient.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxDbClient.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxDbClient.o: /usr/include/boost/config/select_platform_config.hpp
hoxDbClient.o: /usr/include/boost/config/posix_features.hpp
hoxDbClient.o: /usr/include/unistd.h /usr/include/features.h
hoxDbClient.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxDbClient.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxDbClient.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
hoxDbClient.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
hoxDbClient.o: /usr/include/getopt.h /usr/include/boost/config/suffix.hpp
hoxDbClient.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxDbClient.o: /usr/include/boost/checked_delete.hpp
hoxDbClient.o: /usr/include/boost/throw_exception.hpp
hoxDbClient.o: /usr/include/boost/config.hpp
hoxDbClient.o: /usr/include/boost/detail/shared_count.hpp
hoxDbClient.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxDbClient.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxDbClient.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxDbClient.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxDbClient.o: /usr/include/boost/detail/workaround.hpp ../include/st.h
hoxDbClient.o: /usr/include/sys/types.h /usr/include/time.h
hoxDbClient.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxDbClient.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
hoxDbClient.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
hoxDbClient.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
hoxDbClient.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
hoxDbClient.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
hoxDbClient.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
hoxDbClient.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
hoxDbClient.o: /usr/include/errno.h /usr/include/bits/errno.h
hoxDbClient.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
hoxDbClient.o: /usr/include/asm-generic/errno.h
hoxDbClient.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxDbClient.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxEnums.h
hoxDbClient.o: hoxDebug.h hoxLog.h hoxPlayer.h
hoxDbClient.o: /usr/include/boost/enable_shared_from_this.hpp
hoxDbClient.o: /usr/include/boost/weak_ptr.hpp hoxUtil.h hoxSocketAPI.h
hoxDbClient.o: /usr/include/netdb.h /usr/include/netinet/in.h
hoxDbClient.o: /usr/include/stdint.h /usr/include/bits/wchar.h
hoxDbClient.o: /usr/include/bits/in.h /usr/include/rpc/netdb.h
hoxDbClient.o: /usr/include/bits/netdb.h /usr/include/boost/tokenizer.hpp
hoxDbClient.o: /usr/include/boost/token_iterator.hpp
hoxDbClient.o: /usr/include/boost/iterator/iterator_adaptor.hpp
hoxDbClient.o: /usr/include/boost/static_assert.hpp
hoxDbClient.o: /usr/include/boost/iterator.hpp
hoxDbClient.o: /usr/include/boost/detail/iterator.hpp
hoxDbClient.o: /usr/include/boost/iterator/iterator_categories.hpp
hoxDbClient.o: /usr/include/boost/iterator/detail/config_def.hpp
hoxDbClient.o: /usr/include/boost/mpl/eval_if.hpp
hoxDbClient.o: /usr/include/boost/mpl/if.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/value_wknd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/static_cast.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/workaround.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/integral.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/msvc.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/eti.hpp
hoxDbClient.o: /usr/include/boost/mpl/int.hpp
hoxDbClient.o: /usr/include/boost/mpl/int_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/adl_barrier.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/adl.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/intel.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/gcc.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/nttp_decl.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/nttp.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/cat.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/config/config.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/integral_wrapper.hpp
hoxDbClient.o: /usr/include/boost/mpl/integral_c_tag.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/static_constant.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/na_spec.hpp
hoxDbClient.o: /usr/include/boost/mpl/lambda_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/void_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/na.hpp
hoxDbClient.o: /usr/include/boost/mpl/bool.hpp
hoxDbClient.o: /usr/include/boost/mpl/bool_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/na_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/ctps.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/lambda.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/ttp.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/lambda_arity_param.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/template_arity_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/arity.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/dtp.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/preprocessor/params.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/preprocessor.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/comma_if.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/punctuation/comma_if.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/control/if.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/control/iif.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/logical/bool.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/facilities/empty.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/punctuation/comma.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/repeat.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/repetition/repeat.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/debug/error.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/detail/auto_rec.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/tuple/eat.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/inc.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/arithmetic/inc.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/preprocessor/enum.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/preprocessor/def_params_tail.hpp
hoxDbClient.o: /usr/include/boost/mpl/limits/arity.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/logical/and.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/logical/bitand.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/identity.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/facilities/identity.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/empty.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/arithmetic/add.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/arithmetic/dec.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/control/while.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/list/fold_left.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/list/detail/fold_left.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/control/expr_iif.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/list/adt.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/detail/is_binary.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/detail/check.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/logical/compl.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/list/fold_right.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/list/detail/fold_right.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/list/reverse.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/control/detail/while.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/tuple/elem.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/arithmetic/sub.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/overload_resolution.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/lambda_support.hpp
hoxDbClient.o: /usr/include/boost/mpl/identity.hpp
hoxDbClient.o: /usr/include/boost/mpl/placeholders.hpp
hoxDbClient.o: /usr/include/boost/mpl/arg.hpp
hoxDbClient.o: /usr/include/boost/mpl/arg_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/na_assert.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/arity_spec.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/arg_typedef.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/use_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/compiler.hpp
hoxDbClient.o: /usr/include/boost/preprocessor/stringize.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_convertible.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/yes_no_type.hpp
hoxDbClient.o: /usr/include/boost/type_traits/config.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_array.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/bool_trait_def.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/template_arity_spec.hpp
hoxDbClient.o: /usr/include/boost/type_traits/integral_constant.hpp
hoxDbClient.o: /usr/include/boost/mpl/integral_c.hpp
hoxDbClient.o: /usr/include/boost/mpl/integral_c_fwd.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/bool_trait_undef.hpp
hoxDbClient.o: /usr/include/boost/type_traits/add_reference.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_reference.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/type_trait_def.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/type_trait_undef.hpp
hoxDbClient.o: /usr/include/boost/type_traits/ice.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/ice_or.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/ice_and.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/ice_not.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/ice_eq.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_arithmetic.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_integral.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_float.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_void.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_abstract.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_class.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_union.hpp
hoxDbClient.o: /usr/include/boost/type_traits/remove_cv.hpp
hoxDbClient.o: /usr/include/boost/type_traits/broken_compiler_spec.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/cv_traits_impl.hpp
hoxDbClient.o: /usr/include/boost/type_traits/intrinsics.hpp
hoxDbClient.o: /usr/include/boost/iterator/detail/config_undef.hpp
hoxDbClient.o: /usr/include/boost/iterator/iterator_facade.hpp
hoxDbClient.o: /usr/include/boost/iterator/interoperable.hpp
hoxDbClient.o: /usr/include/boost/mpl/or.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/nested_type_wknd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/iterator/iterator_traits.hpp
hoxDbClient.o: /usr/include/boost/iterator/detail/facade_iterator_category.hpp
hoxDbClient.o: /usr/include/boost/mpl/and.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_same.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_const.hpp
hoxDbClient.o: /usr/include/boost/detail/indirect_traits.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_function.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/false_result.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/is_function_ptr_helper.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_pointer.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_member_pointer.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_member_function_pointer.hpp
hoxDbClient.o: /usr/include/boost/type_traits/detail/is_mem_fun_pointer_impl.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_volatile.hpp
hoxDbClient.o: /usr/include/boost/type_traits/remove_reference.hpp
hoxDbClient.o: /usr/include/boost/type_traits/remove_pointer.hpp
hoxDbClient.o: /usr/include/boost/mpl/not.hpp
hoxDbClient.o: /usr/include/boost/iterator/detail/enable_if.hpp
hoxDbClient.o: /usr/include/boost/implicit_cast.hpp
hoxDbClient.o: /usr/include/boost/type_traits/add_const.hpp
hoxDbClient.o: /usr/include/boost/type_traits/add_pointer.hpp
hoxDbClient.o: /usr/include/boost/type_traits/remove_const.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_pod.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_scalar.hpp
hoxDbClient.o: /usr/include/boost/type_traits/is_enum.hpp
hoxDbClient.o: /usr/include/boost/mpl/always.hpp
hoxDbClient.o: /usr/include/boost/mpl/apply.hpp
hoxDbClient.o: /usr/include/boost/mpl/apply_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/apply_wrap.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/has_apply.hpp
hoxDbClient.o: /usr/include/boost/mpl/has_xxx.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/type_wrapper.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/yes_no.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/arrays.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/has_xxx.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/msvc_typename.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/has_apply.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/msvc_never_true.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/lambda.hpp
hoxDbClient.o: /usr/include/boost/mpl/bind.hpp
hoxDbClient.o: /usr/include/boost/mpl/bind_fwd.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/config/bind.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/next.hpp
hoxDbClient.o: /usr/include/boost/mpl/next_prior.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/common_name_wknd.hpp
hoxDbClient.o: /usr/include/boost/mpl/protect.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/full_lambda.hpp
hoxDbClient.o: /usr/include/boost/mpl/quote.hpp
hoxDbClient.o: /usr/include/boost/mpl/void.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/has_type.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/template_arity.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
hoxDbClient.o: /usr/include/boost/iterator/detail/minimum_category.hpp
hoxDbClient.o: /usr/include/boost/token_functions.hpp
hoxFileMgr.o: hoxFileMgr.h hoxTypes.h /usr/include/boost/shared_ptr.hpp
hoxFileMgr.o: /usr/include/boost/config.hpp
hoxFileMgr.o: /usr/include/boost/config/user.hpp
hoxFileMgr.o: /usr/include/boost/config/select_compiler_config.hpp
hoxFileMgr.o: /usr/include/boost/config/compiler/gcc.hpp
hoxFileMgr.o: /usr/include/boost/config/select_stdlib_config.hpp
hoxFileMgr.o: /usr/include/boost/config/no_tr1/utility.hpp
hoxFileMgr.o: /usr/include/boost/config/select_platform_config.hpp
hoxFileMgr.o: /usr/include/boost/config/posix_features.hpp
hoxFileMgr.o: /usr/include/unistd.h /usr/include/features.h
hoxFileMgr.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
hoxFileMgr.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
hoxFileMgr.o: /usr/include/bits/posix_opt.h /usr/include/bits/types.h
hoxFileMgr.o: /usr/include/bits/typesizes.h /usr/include/bits/confname.h
hoxFileMgr.o: /usr/include/getopt.h /usr/include/boost/config/suffix.hpp
hoxFileMgr.o: /usr/include/boost/assert.hpp /usr/include/assert.h
hoxFileMgr.o: /usr/include/boost/checked_delete.hpp
hoxFileMgr.o: /usr/include/boost/throw_exception.hpp
hoxFileMgr.o: /usr/include/boost/config.hpp
hoxFileMgr.o: /usr/include/boost/detail/shared_count.hpp
hoxFileMgr.o: /usr/include/boost/detail/bad_weak_ptr.hpp
hoxFileMgr.o: /usr/include/boost/detail/sp_counted_base.hpp
hoxFileMgr.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
hoxFileMgr.o: /usr/include/boost/detail/sp_counted_impl.hpp
hoxFileMgr.o: /usr/include/boost/detail/workaround.hpp ../include/st.h
hoxFileMgr.o: /usr/include/sys/types.h /usr/include/time.h
hoxFileMgr.o: /usr/include/endian.h /usr/include/bits/endian.h
hoxFileMgr.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
hoxFileMgr.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
hoxFileMgr.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
hoxFileMgr.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/socket.h
hoxFileMgr.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
hoxFileMgr.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
hoxFileMgr.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
hoxFileMgr.o: /usr/include/errno.h /usr/include/bits/errno.h
hoxFileMgr.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
hoxFileMgr.o: /usr/include/asm-generic/errno.h
hoxFileMgr.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
hoxFileMgr.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxEnums.h
hoxFileMgr.o: hoxDebug.h hoxDbClient.h hoxExcept.h hoxLog.h
session.o: /usr/include/netinet/in.h /usr/include/features.h
session.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
session.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
session.o: /usr/include/stdint.h /usr/include/bits/wchar.h
session.o: /usr/include/sys/socket.h /usr/include/sys/uio.h
session.o: /usr/include/sys/types.h /usr/include/bits/types.h
session.o: /usr/include/bits/typesizes.h /usr/include/time.h
session.o: /usr/include/endian.h /usr/include/bits/endian.h
session.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
session.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
session.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
session.o: /usr/include/bits/pthreadtypes.h /usr/include/bits/uio.h
session.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
session.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
session.o: /usr/include/bits/in.h /usr/include/arpa/inet.h ../include/st.h
session.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
session.o: /usr/include/bits/confname.h /usr/include/getopt.h
session.o: /usr/include/errno.h /usr/include/bits/errno.h
session.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
session.o: /usr/include/asm-generic/errno.h
session.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
session.o: /usr/include/sys/poll.h /usr/include/bits/poll.h hoxEnums.h
session.o: hoxLog.h hoxExcept.h hoxDebug.h hoxTypes.h
session.o: /usr/include/boost/shared_ptr.hpp /usr/include/boost/config.hpp
session.o: /usr/include/boost/config/user.hpp
session.o: /usr/include/boost/config/select_compiler_config.hpp
session.o: /usr/include/boost/config/compiler/gcc.hpp
session.o: /usr/include/boost/config/select_stdlib_config.hpp
session.o: /usr/include/boost/config/no_tr1/utility.hpp
session.o: /usr/include/boost/config/select_platform_config.hpp
session.o: /usr/include/boost/config/posix_features.hpp
session.o: /usr/include/boost/config/suffix.hpp /usr/include/boost/assert.hpp
session.o: /usr/include/assert.h /usr/include/boost/checked_delete.hpp
session.o: /usr/include/boost/throw_exception.hpp
session.o: /usr/include/boost/config.hpp
session.o: /usr/include/boost/detail/shared_count.hpp
session.o: /usr/include/boost/detail/bad_weak_ptr.hpp
session.o: /usr/include/boost/detail/sp_counted_base.hpp
session.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
session.o: /usr/include/boost/detail/sp_counted_impl.hpp
session.o: /usr/include/boost/detail/workaround.hpp hoxSocketAPI.h
session.o: hoxSessionMgr.h hoxSession.h
session.o: /usr/include/boost/enable_shared_from_this.hpp
session.o: /usr/include/boost/weak_ptr.hpp hoxUtil.h hoxTable.h hoxPlayer.h
session.o: hoxDbClient.h hoxFileMgr.h /usr/include/boost/tokenizer.hpp
session.o: /usr/include/boost/token_iterator.hpp
session.o: /usr/include/boost/iterator/iterator_adaptor.hpp
session.o: /usr/include/boost/static_assert.hpp
session.o: /usr/include/boost/iterator.hpp
session.o: /usr/include/boost/detail/iterator.hpp
session.o: /usr/include/boost/iterator/iterator_categories.hpp
session.o: /usr/include/boost/iterator/detail/config_def.hpp
session.o: /usr/include/boost/mpl/eval_if.hpp /usr/include/boost/mpl/if.hpp
session.o: /usr/include/boost/mpl/aux_/value_wknd.hpp
session.o: /usr/include/boost/mpl/aux_/static_cast.hpp
session.o: /usr/include/boost/mpl/aux_/config/workaround.hpp
session.o: /usr/include/boost/mpl/aux_/config/integral.hpp
session.o: /usr/include/boost/mpl/aux_/config/msvc.hpp
session.o: /usr/include/boost/mpl/aux_/config/eti.hpp
session.o: /usr/include/boost/mpl/int.hpp /usr/include/boost/mpl/int_fwd.hpp
session.o: /usr/include/boost/mpl/aux_/adl_barrier.hpp
session.o: /usr/include/boost/mpl/aux_/config/adl.hpp
session.o: /usr/include/boost/mpl/aux_/config/intel.hpp
session.o: /usr/include/boost/mpl/aux_/config/gcc.hpp
session.o: /usr/include/boost/mpl/aux_/nttp_decl.hpp
session.o: /usr/include/boost/mpl/aux_/config/nttp.hpp
session.o: /usr/include/boost/preprocessor/cat.hpp
session.o: /usr/include/boost/preprocessor/config/config.hpp
session.o: /usr/include/boost/mpl/aux_/integral_wrapper.hpp
session.o: /usr/include/boost/mpl/integral_c_tag.hpp
session.o: /usr/include/boost/mpl/aux_/config/static_constant.hpp
session.o: /usr/include/boost/mpl/aux_/na_spec.hpp
session.o: /usr/include/boost/mpl/lambda_fwd.hpp
session.o: /usr/include/boost/mpl/void_fwd.hpp
session.o: /usr/include/boost/mpl/aux_/na.hpp /usr/include/boost/mpl/bool.hpp
session.o: /usr/include/boost/mpl/bool_fwd.hpp
session.o: /usr/include/boost/mpl/aux_/na_fwd.hpp
session.o: /usr/include/boost/mpl/aux_/config/ctps.hpp
session.o: /usr/include/boost/mpl/aux_/config/lambda.hpp
session.o: /usr/include/boost/mpl/aux_/config/ttp.hpp
session.o: /usr/include/boost/mpl/aux_/lambda_arity_param.hpp
session.o: /usr/include/boost/mpl/aux_/template_arity_fwd.hpp
session.o: /usr/include/boost/mpl/aux_/arity.hpp
session.o: /usr/include/boost/mpl/aux_/config/dtp.hpp
session.o: /usr/include/boost/mpl/aux_/preprocessor/params.hpp
session.o: /usr/include/boost/mpl/aux_/config/preprocessor.hpp
session.o: /usr/include/boost/preprocessor/comma_if.hpp
session.o: /usr/include/boost/preprocessor/punctuation/comma_if.hpp
session.o: /usr/include/boost/preprocessor/control/if.hpp
session.o: /usr/include/boost/preprocessor/control/iif.hpp
session.o: /usr/include/boost/preprocessor/logical/bool.hpp
session.o: /usr/include/boost/preprocessor/facilities/empty.hpp
session.o: /usr/include/boost/preprocessor/punctuation/comma.hpp
session.o: /usr/include/boost/preprocessor/repeat.hpp
session.o: /usr/include/boost/preprocessor/repetition/repeat.hpp
session.o: /usr/include/boost/preprocessor/debug/error.hpp
session.o: /usr/include/boost/preprocessor/detail/auto_rec.hpp
session.o: /usr/include/boost/preprocessor/tuple/eat.hpp
session.o: /usr/include/boost/preprocessor/inc.hpp
session.o: /usr/include/boost/preprocessor/arithmetic/inc.hpp
session.o: /usr/include/boost/mpl/aux_/preprocessor/enum.hpp
session.o: /usr/include/boost/mpl/aux_/preprocessor/def_params_tail.hpp
session.o: /usr/include/boost/mpl/limits/arity.hpp
session.o: /usr/include/boost/preprocessor/logical/and.hpp
session.o: /usr/include/boost/preprocessor/logical/bitand.hpp
session.o: /usr/include/boost/preprocessor/identity.hpp
session.o: /usr/include/boost/preprocessor/facilities/identity.hpp
session.o: /usr/include/boost/preprocessor/empty.hpp
session.o: /usr/include/boost/preprocessor/arithmetic/add.hpp
session.o: /usr/include/boost/preprocessor/arithmetic/dec.hpp
session.o: /usr/include/boost/preprocessor/control/while.hpp
session.o: /usr/include/boost/preprocessor/list/fold_left.hpp
session.o: /usr/include/boost/preprocessor/list/detail/fold_left.hpp
session.o: /usr/include/boost/preprocessor/control/expr_iif.hpp
session.o: /usr/include/boost/preprocessor/list/adt.hpp
session.o: /usr/include/boost/preprocessor/detail/is_binary.hpp
session.o: /usr/include/boost/preprocessor/detail/check.hpp
session.o: /usr/include/boost/preprocessor/logical/compl.hpp
session.o: /usr/include/boost/preprocessor/list/fold_right.hpp
session.o: /usr/include/boost/preprocessor/list/detail/fold_right.hpp
session.o: /usr/include/boost/preprocessor/list/reverse.hpp
session.o: /usr/include/boost/preprocessor/control/detail/while.hpp
session.o: /usr/include/boost/preprocessor/tuple/elem.hpp
session.o: /usr/include/boost/preprocessor/arithmetic/sub.hpp
session.o: /usr/include/boost/mpl/aux_/config/overload_resolution.hpp
session.o: /usr/include/boost/mpl/aux_/lambda_support.hpp
session.o: /usr/include/boost/mpl/identity.hpp
session.o: /usr/include/boost/mpl/placeholders.hpp
session.o: /usr/include/boost/mpl/arg.hpp /usr/include/boost/mpl/arg_fwd.hpp
session.o: /usr/include/boost/mpl/aux_/na_assert.hpp
session.o: /usr/include/boost/mpl/aux_/arity_spec.hpp
session.o: /usr/include/boost/mpl/aux_/arg_typedef.hpp
session.o: /usr/include/boost/mpl/aux_/config/use_preprocessed.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/mpl/aux_/config/compiler.hpp
session.o: /usr/include/boost/preprocessor/stringize.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/type_traits/is_convertible.hpp
session.o: /usr/include/boost/type_traits/detail/yes_no_type.hpp
session.o: /usr/include/boost/type_traits/config.hpp
session.o: /usr/include/boost/type_traits/is_array.hpp
session.o: /usr/include/boost/type_traits/detail/bool_trait_def.hpp
session.o: /usr/include/boost/type_traits/detail/template_arity_spec.hpp
session.o: /usr/include/boost/type_traits/integral_constant.hpp
session.o: /usr/include/boost/mpl/integral_c.hpp
session.o: /usr/include/boost/mpl/integral_c_fwd.hpp
session.o: /usr/include/boost/type_traits/detail/bool_trait_undef.hpp
session.o: /usr/include/boost/type_traits/add_reference.hpp
session.o: /usr/include/boost/type_traits/is_reference.hpp
session.o: /usr/include/boost/type_traits/detail/type_trait_def.hpp
session.o: /usr/include/boost/type_traits/detail/type_trait_undef.hpp
session.o: /usr/include/boost/type_traits/ice.hpp
session.o: /usr/include/boost/type_traits/detail/ice_or.hpp
session.o: /usr/include/boost/type_traits/detail/ice_and.hpp
session.o: /usr/include/boost/type_traits/detail/ice_not.hpp
session.o: /usr/include/boost/type_traits/detail/ice_eq.hpp
session.o: /usr/include/boost/type_traits/is_arithmetic.hpp
session.o: /usr/include/boost/type_traits/is_integral.hpp
session.o: /usr/include/boost/type_traits/is_float.hpp
session.o: /usr/include/boost/type_traits/is_void.hpp
session.o: /usr/include/boost/type_traits/is_abstract.hpp
session.o: /usr/include/boost/type_traits/is_class.hpp
session.o: /usr/include/boost/type_traits/is_union.hpp
session.o: /usr/include/boost/type_traits/remove_cv.hpp
session.o: /usr/include/boost/type_traits/broken_compiler_spec.hpp
session.o: /usr/include/boost/type_traits/detail/cv_traits_impl.hpp
session.o: /usr/include/boost/type_traits/intrinsics.hpp
session.o: /usr/include/boost/iterator/detail/config_undef.hpp
session.o: /usr/include/boost/iterator/iterator_facade.hpp
session.o: /usr/include/boost/iterator/interoperable.hpp
session.o: /usr/include/boost/mpl/or.hpp
session.o: /usr/include/boost/mpl/aux_/nested_type_wknd.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/iterator/iterator_traits.hpp
session.o: /usr/include/boost/iterator/detail/facade_iterator_category.hpp
session.o: /usr/include/boost/mpl/and.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/type_traits/is_same.hpp
session.o: /usr/include/boost/type_traits/is_const.hpp
session.o: /usr/include/boost/detail/indirect_traits.hpp
session.o: /usr/include/boost/type_traits/is_function.hpp
session.o: /usr/include/boost/type_traits/detail/false_result.hpp
session.o: /usr/include/boost/type_traits/detail/is_function_ptr_helper.hpp
session.o: /usr/include/boost/type_traits/is_pointer.hpp
session.o: /usr/include/boost/type_traits/is_member_pointer.hpp
session.o: /usr/include/boost/type_traits/is_member_function_pointer.hpp
session.o: /usr/include/boost/type_traits/detail/is_mem_fun_pointer_impl.hpp
session.o: /usr/include/boost/type_traits/is_volatile.hpp
session.o: /usr/include/boost/type_traits/remove_reference.hpp
session.o: /usr/include/boost/type_traits/remove_pointer.hpp
session.o: /usr/include/boost/mpl/not.hpp
session.o: /usr/include/boost/iterator/detail/enable_if.hpp
session.o: /usr/include/boost/implicit_cast.hpp
session.o: /usr/include/boost/type_traits/add_const.hpp
session.o: /usr/include/boost/type_traits/add_pointer.hpp
session.o: /usr/include/boost/type_traits/remove_const.hpp
session.o: /usr/include/boost/type_traits/is_pod.hpp
session.o: /usr/include/boost/type_traits/is_scalar.hpp
session.o: /usr/include/boost/type_traits/is_enum.hpp
session.o: /usr/include/boost/mpl/always.hpp /usr/include/boost/mpl/apply.hpp
session.o: /usr/include/boost/mpl/apply_fwd.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/mpl/apply_wrap.hpp
session.o: /usr/include/boost/mpl/aux_/has_apply.hpp
session.o: /usr/include/boost/mpl/has_xxx.hpp
session.o: /usr/include/boost/mpl/aux_/type_wrapper.hpp
session.o: /usr/include/boost/mpl/aux_/yes_no.hpp
session.o: /usr/include/boost/mpl/aux_/config/arrays.hpp
session.o: /usr/include/boost/mpl/aux_/config/has_xxx.hpp
session.o: /usr/include/boost/mpl/aux_/config/msvc_typename.hpp
session.o: /usr/include/boost/mpl/aux_/config/has_apply.hpp
session.o: /usr/include/boost/mpl/aux_/msvc_never_true.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/mpl/lambda.hpp /usr/include/boost/mpl/bind.hpp
session.o: /usr/include/boost/mpl/bind_fwd.hpp
session.o: /usr/include/boost/mpl/aux_/config/bind.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/mpl/next.hpp
session.o: /usr/include/boost/mpl/next_prior.hpp
session.o: /usr/include/boost/mpl/aux_/common_name_wknd.hpp
session.o: /usr/include/boost/mpl/protect.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/mpl/aux_/full_lambda.hpp
session.o: /usr/include/boost/mpl/quote.hpp /usr/include/boost/mpl/void.hpp
session.o: /usr/include/boost/mpl/aux_/has_type.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/mpl/aux_/template_arity.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/mpl/aux_/include_preprocessed.hpp
session.o: /usr/include/boost/iterator/detail/minimum_category.hpp
session.o: /usr/include/boost/token_functions.hpp
main.o: /usr/include/sys/types.h /usr/include/features.h
main.o: /usr/include/sys/cdefs.h /usr/include/bits/wordsize.h
main.o: /usr/include/gnu/stubs.h /usr/include/gnu/stubs-32.h
main.o: /usr/include/bits/types.h /usr/include/bits/typesizes.h
main.o: /usr/include/time.h /usr/include/endian.h /usr/include/bits/endian.h
main.o: /usr/include/bits/byteswap.h /usr/include/sys/select.h
main.o: /usr/include/bits/select.h /usr/include/bits/sigset.h
main.o: /usr/include/bits/time.h /usr/include/sys/sysmacros.h
main.o: /usr/include/bits/pthreadtypes.h /usr/include/sys/stat.h
main.o: /usr/include/bits/stat.h /usr/include/sys/socket.h
main.o: /usr/include/sys/uio.h /usr/include/bits/uio.h
main.o: /usr/include/bits/socket.h /usr/include/bits/sockaddr.h
main.o: /usr/include/asm/socket.h /usr/include/asm/sockios.h
main.o: /usr/include/sys/wait.h /usr/include/signal.h
main.o: /usr/include/bits/signum.h /usr/include/bits/siginfo.h
main.o: /usr/include/bits/sigaction.h /usr/include/bits/sigcontext.h
main.o: /usr/include/bits/sigstack.h /usr/include/bits/sigthread.h
main.o: /usr/include/sys/resource.h /usr/include/bits/resource.h
main.o: /usr/include/bits/waitflags.h /usr/include/bits/waitstatus.h
main.o: /usr/include/netinet/in.h /usr/include/stdint.h
main.o: /usr/include/bits/wchar.h /usr/include/bits/in.h
main.o: /usr/include/arpa/inet.h /usr/include/netdb.h
main.o: /usr/include/rpc/netdb.h /usr/include/bits/netdb.h
main.o: /usr/include/fcntl.h /usr/include/bits/fcntl.h /usr/include/pwd.h
main.o: /usr/include/stdio.h /usr/include/execinfo.h ../include/st.h
main.o: /usr/include/unistd.h /usr/include/bits/posix_opt.h
main.o: /usr/include/bits/confname.h /usr/include/getopt.h
main.o: /usr/include/errno.h /usr/include/bits/errno.h
main.o: /usr/include/linux/errno.h /usr/include/asm/errno.h
main.o: /usr/include/asm-generic/errno.h
main.o: /usr/include/asm-generic/errno-base.h /usr/include/poll.h
main.o: /usr/include/sys/poll.h /usr/include/bits/poll.h
main.o: ../include/libconfig.h++ ../include/libconfig.h main.h hoxLog.h
main.o: hoxEnums.h hoxTypes.h /usr/include/boost/shared_ptr.hpp
main.o: /usr/include/boost/config.hpp /usr/include/boost/config/user.hpp
main.o: /usr/include/boost/config/select_compiler_config.hpp
main.o: /usr/include/boost/config/compiler/gcc.hpp
main.o: /usr/include/boost/config/select_stdlib_config.hpp
main.o: /usr/include/boost/config/no_tr1/utility.hpp
main.o: /usr/include/boost/config/select_platform_config.hpp
main.o: /usr/include/boost/config/posix_features.hpp
main.o: /usr/include/boost/config/suffix.hpp /usr/include/boost/assert.hpp
main.o: /usr/include/assert.h /usr/include/boost/checked_delete.hpp
main.o: /usr/include/boost/throw_exception.hpp /usr/include/boost/config.hpp
main.o: /usr/include/boost/detail/shared_count.hpp
main.o: /usr/include/boost/detail/bad_weak_ptr.hpp
main.o: /usr/include/boost/detail/sp_counted_base.hpp
main.o: /usr/include/boost/detail/sp_counted_base_gcc_x86.hpp
main.o: /usr/include/boost/detail/sp_counted_impl.hpp
main.o: /usr/include/boost/detail/workaround.hpp hoxDebug.h hoxDbClient.h
main.o: hoxFileMgr.h

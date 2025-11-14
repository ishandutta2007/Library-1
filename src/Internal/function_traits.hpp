#pragma once
#include <type_traits>
// clang-format off
namespace function_template_internal{
template<class C>struct is_function_object{
 template<class U,int dummy=(&U::operator(),0)> static std::true_type check(U *);
 static std::false_type check(...);
 static C *m;
 static constexpr bool value= decltype(check(m))::value;
};
template<class F,bool,bool>struct function_type_impl{using type= void;};
template<class F>struct function_type_impl<F,true,false>{using type= F *;};
template<class F>struct function_type_impl<F,false,true>{using type= decltype(&F::operator());};
template<class F> using function_type_t= typename function_type_impl<F,std::is_function_v<F>,is_function_object<F>::value>::type;
template<class... Args>struct result_type_impl{using type= void;};
template<class R,class... Args>struct result_type_impl<R(*)(Args...)>{using type= R;};
template<class C,class R,class... Args>struct result_type_impl<R(C::*)(Args...)>{using type= R;};
template<class C,class R,class... Args>struct result_type_impl<R(C::*)(Args...)const>{using type= R;};
template<class F> using result_type_t= typename result_type_impl<function_type_t<F>>::type;
template<class... Args>struct argument_type_impl{using type= void;};
template<class R,class... Args>struct argument_type_impl<R(*)(Args...)>{using type= std::tuple<Args...>;};
template<class C,class R,class... Args>struct argument_type_impl<R(C::*)(Args...)>{using type= std::tuple<Args...>;};
template<class C,class R,class... Args>struct argument_type_impl<R(C::*)(Args...)const>{using type= std::tuple<Args...>;};
template<class F> using argument_type_t= typename argument_type_impl<function_type_t<F>>::type;
}
using function_template_internal::result_type_t,function_template_internal::argument_type_t;
// clang-format on
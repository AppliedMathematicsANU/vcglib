
#include <boost/version.hpp>

#if BOOST_VERSION < 106500
#include <boost/python/numeric.hpp>
#else  // #if BOOST_VERSION < 106500
#include <boost/python/numpy.hpp>
#endif

#include <boost/python.hpp>

namespace vcg
{
namespace python
{

#if BOOST_VERSION < 106500

namespace bpnp = ::boost::python::numeric;
using BpNumpyArray = typename ::boost::python::numeric::array;

inline void set_module_and_type_wrapper()
{
    BpNumpyArray::set_module_and_type("numpy", "ndarray");
}

#else

namespace bpnp = ::boost::python::numpy;
using BpNumpyArray = typename boost::python::numpy::ndarray;

inline void set_module_and_type_wrapper() {}

#endif
}
}

BOOST_PYTHON_MODULE(_tridecimator)
{
    vcg::python::bpnp::initialize();
}


#include "FunctFun/functfun.hpp"



auto main()->int
{
    functfun::functfun_BasicTest();
    functfun::countlines_Test();
    functfun::slidingMean_Test();
    functfun::makeUniformDistribution_Test();
    functfun::slidingMean_ChronoDurTest();
    functfun::stringTrim_Test();
    functfun::filterTransform_Test();
    functfun::viewsfilterTransform_Test();
    functfun::jsonData_Test();
    functfun::join_Test();
    functfun::joinStrings_Test();
    functfun::joinMapToString_Test();
    functfun::rangesJoin_Examples();
    functfun::joinFuncObj_Test();
    functfun::splitview_Test();
    functfun::mapview_Test();
    functfun::expressionTemplates_Test();
    functfun::joinwithview_Test();
    functfun::joinwith_runtimeTest();
    functfun::transpose_trial_Test();
    functfun::cartesianproductview_Test();

}

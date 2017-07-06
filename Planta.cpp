//
// File: Planta.cpp
//
// Code generated for Simulink model 'Planta'.
//
// Model version                  : 1.70
// Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
// C/C++ source code generated on : Sun Jul  2 09:28:57 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: 32-bit Generic
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "Planta.h"
#include "Planta_private.h"

static void rate_scheduler(RT_MODEL_Planta_T *const Planta_M);

//
//   This function updates active task flag for each subrate.
// The function is called at model base rate, hence the
// generated code self-manages all its subrates.
//
static void rate_scheduler(RT_MODEL_Planta_T *const Planta_M)
{
  // Compute which subrates run during the next base time step.  Subrates
  //  are an integer multiple of the base rate counter.  Therefore, the subtask
  //  counter is reset when it reaches its limit (zero means run).

  (Planta_M->Timing.TaskCounters.TID[1])++;
  if ((Planta_M->Timing.TaskCounters.TID[1]) > 1) {// Sample time: [1.0s, 0.0s]
    Planta_M->Timing.TaskCounters.TID[1] = 0;
  }
}

// Model step function
void PlantaModelClass::step(real_T &arg_Out1, const real_T *arg_Ref_in, const
  real_T *arg_Inicial)
{
  real_T denAccum;
  if ((&Planta_M)->Timing.TaskCounters.TID[1] == 0) {
    // Gain: '<Root>/Gain3' incorporates:
    //   Inport: '<Root>/Inicial'

    Planta_DW.Gain3 = Planta_P.Gain3_Gain * *arg_Inicial;

    // Gain: '<Root>/Gain'
    Planta_DW.Gain = Planta_P.Gain_Gain * Planta_DW.Gain3;

    // DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn'
    if (Planta_DW.DiscreteTransferFcn_icLoad != 0) {
      Planta_DW.DiscreteTransferFcn_states = Planta_DW.Gain;
      Planta_DW.DiscreteTransferFcn_icLoad = 0U;
    }

    denAccum = Planta_P.DiscreteTransferFcn_NumCoef *
      Planta_DW.DiscreteTransferFcn_states;
    Planta_DW.DiscreteTransferFcn = denAccum;

    // End of DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn'

    // Outport: '<Root>/Out1' incorporates:
    //   Gain: '<Root>/Gain2'

    Planta_Y.Out1 = Planta_P.Gain2_Gain * Planta_DW.DiscreteTransferFcn;

    // Gain: '<Root>/Gain1' incorporates:
    //   Inport: '<Root>/Ref_in'

    Planta_DW.Gain1 = Planta_P.Gain1_Gain * *arg_Ref_in;

    // Update for DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn'
    Planta_DW.DiscreteTransferFcn_icLoad = 0U;
    denAccum = Planta_DW.Gain1;
    denAccum -= Planta_P.DiscreteTransferFcn_DenCoef[1] *
      Planta_DW.DiscreteTransferFcn_states;
    denAccum /= Planta_P.DiscreteTransferFcn_DenCoef[0];
    Planta_DW.DiscreteTransferFcn_states = denAccum;
  }

  rate_scheduler((&Planta_M));

  // Copy value for root outport '<Root>/Out1' since it is accessed globally
  arg_Out1 = Planta_Y.Out1;
}

// Model initialize function
void PlantaModelClass::initialize()
{
  // InitializeConditions for DiscreteTransferFcn: '<Root>/Discrete Transfer Fcn' 
  Planta_DW.DiscreteTransferFcn_icLoad = 1U;
}

// Constructor
PlantaModelClass::PlantaModelClass()
{
  P_Planta_T Planta_P_temp = {
    0.018181818181818181,              // Expression: 1/55
                                       //  Referenced by: '<Root>/Gain3'

    43.497172683775553,                // Expression: 1/0.02299
                                       //  Referenced by: '<Root>/Gain'

    0.02299,                           // Expression: [0.02299]
                                       //  Referenced by: '<Root>/Discrete Transfer Fcn'


    //  Expression: [1 -0.977]
    //  Referenced by: '<Root>/Discrete Transfer Fcn'

    { 1.0, -0.977 },
    55.0,                              // Expression: 55
                                       //  Referenced by: '<Root>/Gain2'

    0.018181818181818181               // Expression: 1/55
                                       //  Referenced by: '<Root>/Gain1'

  };                                   // Modifiable parameters

  // Initialize tunable parameters
  Planta_P = Planta_P_temp;
}

// Destructor
PlantaModelClass::~PlantaModelClass()
{
  // Currently there is no destructor body generated.
}

// Block parameters get method
const P_Planta_T & PlantaModelClass::getBlockParameters() const
{
  return Planta_P;
}

// Block parameters set method
void PlantaModelClass::setBlockParameters(const P_Planta_T *pPlanta_P)
{
  Planta_P = *pPlanta_P;
}

// Real-Time Model get method
RT_MODEL_Planta_T * PlantaModelClass::getRTM()
{
  return (&Planta_M);
}

//
// File trailer for generated code.
//
// [EOF]
//

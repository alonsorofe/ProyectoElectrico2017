//
// File: Controlador.cpp
//
// Code generated for Simulink model 'Controlador'.
//
// Model version                  : 1.41
// Simulink Coder version         : 8.6 (R2014a) 27-Dec-2013
// C/C++ source code generated on : Tue Jun 20 21:06:25 2017
//
// Target selection: ert.tlc
// Embedded hardware selection: 32-bit Generic
// Code generation objectives: Unspecified
// Validation result: Not run
//
#include "Controlador.h"
#include "Controlador_private.h"

static void rate_scheduler(RT_MODEL_Controlador_T *const Controlador_M);

//
//   This function updates active task flag for each subrate.
// The function is called at model base rate, hence the
// generated code self-manages all its subrates.
//
static void rate_scheduler(RT_MODEL_Controlador_T *const Controlador_M)
{
  // Compute which subrates run during the next base time step.  Subrates
  //  are an integer multiple of the base rate counter.  Therefore, the subtask
  //  counter is reset when it reaches its limit (zero means run).

  (Controlador_M->Timing.TaskCounters.TID[1])++;
  if ((Controlador_M->Timing.TaskCounters.TID[1]) > 1) {// Sample time: [1.0s, 0.0s] 
    Controlador_M->Timing.TaskCounters.TID[1] = 0;
  }
}

// Model step function
void ControladorModelClass::step(const real_T *arg_Ref_in, const real_T
  *arg_Retro_in, const real_T *arg_Kp, const real_T *arg_Ki, real_T *arg_U)
{
  if ((&Controlador_M)->Timing.TaskCounters.TID[1] == 0) {
    // Gain: '<Root>/Gain2' incorporates:
    //   Inport: '<Root>/Ref_in'

    Controlador_DW.Gain2 = Controlador_P.Gain2_Gain * *arg_Ref_in;

    // Gain: '<Root>/Gain1' incorporates:
    //   Inport: '<Root>/Retro_in'

    Controlador_DW.Gain1 = Controlador_P.Gain1_Gain * *arg_Retro_in;

    // Sum: '<Root>/Sum2'
    Controlador_DW.Sum2 = Controlador_DW.Gain2 - Controlador_DW.Gain1;

    // Product: '<Root>/Product1' incorporates:
    //   Inport: '<Root>/Kp'

    Controlador_DW.Product1 = Controlador_DW.Sum2 * *arg_Kp;

    // Product: '<Root>/Product2' incorporates:
    //   Inport: '<Root>/Ki'

    Controlador_DW.Product2 = Controlador_DW.Sum2 * *arg_Ki;

    // DiscreteIntegrator: '<Root>/Discrete-Time Integrator1'
    if (Controlador_DW.DiscreteTimeIntegrator1_SYSTEM_ != 0) {
      Controlador_DW.DiscreteTimeIntegrator1 =
        Controlador_DW.DiscreteTimeIntegrator1_DSTATE;
    } else {
      Controlador_DW.DiscreteTimeIntegrator1 =
        Controlador_P.DiscreteTimeIntegrator1_gainval * Controlador_DW.Product2
        + Controlador_DW.DiscreteTimeIntegrator1_DSTATE;
    }

    // End of DiscreteIntegrator: '<Root>/Discrete-Time Integrator1'

    // Sum: '<Root>/Sum3'
    Controlador_DW.Sum3 = Controlador_DW.Product1 +
      Controlador_DW.DiscreteTimeIntegrator1;

    // Outport: '<Root>/U' incorporates:
    //   Gain: '<Root>/Gain5'

    Controlador_Y.U = Controlador_P.Gain5_Gain * Controlador_DW.Sum3;

    // Update for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1'
    Controlador_DW.DiscreteTimeIntegrator1_SYSTEM_ = 0U;
    Controlador_DW.DiscreteTimeIntegrator1_DSTATE =
      Controlador_DW.DiscreteTimeIntegrator1;
  }

  rate_scheduler((&Controlador_M));

  // Copy value for root outport '<Root>/U' since it is accessed globally
  *arg_U = Controlador_Y.U;
}

// Model initialize function
void ControladorModelClass::initialize()
{
  // InitializeConditions for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1' 
  Controlador_DW.DiscreteTimeIntegrator1_DSTATE =
    Controlador_P.DiscreteTimeIntegrator1_IC;

  // Enable for DiscreteIntegrator: '<Root>/Discrete-Time Integrator1'
  Controlador_DW.DiscreteTimeIntegrator1_SYSTEM_ = 1U;
}

// Constructor
ControladorModelClass::ControladorModelClass()
{
  P_Controlador_T Controlador_P_temp = {
    0.025,                             // Expression: 1/40
                                       //  Referenced by: '<Root>/Gain2'

    0.025,                             // Expression: 1/40
                                       //  Referenced by: '<Root>/Gain1'

    1.0,                               // Computed Parameter: DiscreteTimeIntegrator1_gainval
                                       //  Referenced by: '<Root>/Discrete-Time Integrator1'

    0.0,                               // Expression: 0
                                       //  Referenced by: '<Root>/Discrete-Time Integrator1'

    40.0                               // Expression: 40
                                       //  Referenced by: '<Root>/Gain5'

  };                                   // Modifiable parameters

  // Initialize tunable parameters
  Controlador_P = Controlador_P_temp;
}

// Destructor
ControladorModelClass::~ControladorModelClass()
{
  // Currently there is no destructor body generated.
}

// Block parameters get method
const P_Controlador_T & ControladorModelClass::getBlockParameters() const
{
  return Controlador_P;
}

// Block parameters set method
void ControladorModelClass::setBlockParameters(const P_Controlador_T
  *pControlador_P)
{
  Controlador_P = *pControlador_P;
}

// Real-Time Model get method
RT_MODEL_Controlador_T * ControladorModelClass::getRTM()
{
  return (&Controlador_M);
}

//
// File trailer for generated code.
//
// [EOF]
//

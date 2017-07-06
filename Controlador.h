//
// File: Controlador.h
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
#ifndef RTW_HEADER_Controlador_h_
#define RTW_HEADER_Controlador_h_
#ifndef Controlador_COMMON_INCLUDES_
# define Controlador_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // Controlador_COMMON_INCLUDES_

#include "Controlador_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  real_T Gain2;                        // '<Root>/Gain2'
  real_T Gain1;                        // '<Root>/Gain1'
  real_T Sum2;                         // '<Root>/Sum2'
  real_T Product1;                     // '<Root>/Product1'
  real_T Product2;                     // '<Root>/Product2'
  real_T DiscreteTimeIntegrator1;      // '<Root>/Discrete-Time Integrator1'
  real_T Sum3;                         // '<Root>/Sum3'
  real_T DiscreteTimeIntegrator1_DSTATE;// '<Root>/Discrete-Time Integrator1'
  uint8_T DiscreteTimeIntegrator1_SYSTEM_;// '<Root>/Discrete-Time Integrator1'
} DW_Controlador_T;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T U;                            // '<Root>/U'
} ExtY_Controlador_T;

// Parameters (auto storage)
struct P_Controlador_T_ {
  real_T Gain2_Gain;                   // Expression: 1/40
                                       //  Referenced by: '<Root>/Gain2'

  real_T Gain1_Gain;                   // Expression: 1/40
                                       //  Referenced by: '<Root>/Gain1'

  real_T DiscreteTimeIntegrator1_gainval;// Computed Parameter: DiscreteTimeIntegrator1_gainval
                                         //  Referenced by: '<Root>/Discrete-Time Integrator1'

  real_T DiscreteTimeIntegrator1_IC;   // Expression: 0
                                       //  Referenced by: '<Root>/Discrete-Time Integrator1'

  real_T Gain5_Gain;                   // Expression: 40
                                       //  Referenced by: '<Root>/Gain5'

};

// Real-time Model Data Structure
struct tag_RTM_Controlador_T {
  const char_T * volatile errorStatus;

  //
  //  Timing:
  //  The following substructure contains information regarding
  //  the timing information for the model.

  struct {
    struct {
      uint8_T TID[2];
    } TaskCounters;
  } Timing;
};

#ifdef __cplusplus

extern "C" {

#endif

#ifdef __cplusplus

}
#endif

// Class declaration for model Controlador
class ControladorModelClass {
  // public data and function members
 public:
  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step(const real_T *arg_Ref_in, const real_T *arg_Retro_in, const real_T
            *arg_Kp, const real_T *arg_Ki, real_T *arg_U);

  // Constructor
  ControladorModelClass();

  // Destructor
  ~ControladorModelClass();

  // Block parameters get method
  const P_Controlador_T & getBlockParameters() const;

  // Block parameters set method
  void setBlockParameters(const P_Controlador_T *pControlador_P);

  // Real-Time Model get method
  RT_MODEL_Controlador_T * getRTM();

  // protected data and function members
 protected:
  // External outputs
  ExtY_Controlador_T Controlador_Y;

  // private data and function members
 private:
  // Tunable parameters
  P_Controlador_T Controlador_P;

  // Block singals and states
  DW_Controlador_T Controlador_DW;

  // Real-Time Model
  RT_MODEL_Controlador_T Controlador_M;
};

//-
//  The generated code includes comments that allow you to trace directly
//  back to the appropriate location in the model.  The basic format
//  is <system>/block_name, where system is the system number (uniquely
//  assigned by Simulink) and block_name is the name of the block.
//
//  Use the MATLAB hilite_system command to trace the generated code back
//  to the model.  For example,
//
//  hilite_system('<S3>')    - opens system 3
//  hilite_system('<S3>/Kp') - opens and selects block Kp which resides in S3
//
//  Here is the system hierarchy for this model
//
//  '<Root>' : 'Controlador'

#endif                                 // RTW_HEADER_Controlador_h_

//
// File trailer for generated code.
//
// [EOF]
//

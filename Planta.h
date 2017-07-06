//
// File: Planta.h
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
#ifndef RTW_HEADER_Planta_h_
#define RTW_HEADER_Planta_h_
#ifndef Planta_COMMON_INCLUDES_
# define Planta_COMMON_INCLUDES_
#include "rtwtypes.h"
#endif                                 // Planta_COMMON_INCLUDES_

#include "Planta_types.h"

// Macros for accessing real-time model data structure
#ifndef rtmGetErrorStatus
# define rtmGetErrorStatus(rtm)        ((rtm)->errorStatus)
#endif

#ifndef rtmSetErrorStatus
# define rtmSetErrorStatus(rtm, val)   ((rtm)->errorStatus = (val))
#endif

// Block signals and states (auto storage) for system '<Root>'
typedef struct {
  real_T Gain3;                        // '<Root>/Gain3'
  real_T Gain;                         // '<Root>/Gain'
  real_T DiscreteTransferFcn;          // '<Root>/Discrete Transfer Fcn'
  real_T Gain1;                        // '<Root>/Gain1'
  real_T DiscreteTransferFcn_states;   // '<Root>/Discrete Transfer Fcn'
  uint8_T DiscreteTransferFcn_icLoad;  // '<Root>/Discrete Transfer Fcn'
} DW_Planta_T;

// External outputs (root outports fed by signals with auto storage)
typedef struct {
  real_T Out1;                         // '<Root>/Out1'
} ExtY_Planta_T;

// Parameters (auto storage)
struct P_Planta_T_ {
  real_T Gain3_Gain;                   // Expression: 1/55
                                       //  Referenced by: '<Root>/Gain3'

  real_T Gain_Gain;                    // Expression: 1/0.02299
                                       //  Referenced by: '<Root>/Gain'

  real_T DiscreteTransferFcn_NumCoef;  // Expression: [0.02299]
                                       //  Referenced by: '<Root>/Discrete Transfer Fcn'

  real_T DiscreteTransferFcn_DenCoef[2];// Expression: [1 -0.977]
                                        //  Referenced by: '<Root>/Discrete Transfer Fcn'

  real_T Gain2_Gain;                   // Expression: 55
                                       //  Referenced by: '<Root>/Gain2'

  real_T Gain1_Gain;                   // Expression: 1/55
                                       //  Referenced by: '<Root>/Gain1'

};

// Real-time Model Data Structure
struct tag_RTM_Planta_T {
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

// Class declaration for model Planta
class PlantaModelClass {
  // public data and function members
 public:
  // Model entry point functions

  // model initialize function
  void initialize();

  // model step function
  void step(real_T &arg_Out1, const real_T *arg_Ref_in, const real_T
            *arg_Inicial);

  // Constructor
  PlantaModelClass();

  // Destructor
  ~PlantaModelClass();

  // Block parameters get method
  const P_Planta_T & getBlockParameters() const;

  // Block parameters set method
  void setBlockParameters(const P_Planta_T *pPlanta_P);

  // Real-Time Model get method
  RT_MODEL_Planta_T * getRTM();

  // protected data and function members
 protected:
  // External outputs
  ExtY_Planta_T Planta_Y;

  // private data and function members
 private:
  // Tunable parameters
  P_Planta_T Planta_P;

  // Block singals and states
  DW_Planta_T Planta_DW;

  // Real-Time Model
  RT_MODEL_Planta_T Planta_M;
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
//  '<Root>' : 'Planta'

#endif                                 // RTW_HEADER_Planta_h_

//
// File trailer for generated code.
//
// [EOF]
//

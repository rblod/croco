! $Id: set_global_definitions.h 1618 2014-12-18 14:39:51Z rblod $
!
!======================================================================
! CROCO is a branch of ROMS developped at IRD and INRIA, in France
! The two other branches from UCLA (Shchepetkin et al) 
! and Rutgers University (Arango et al) are under MIT/X style license.
! CROCO specific routines (nesting) are under CeCILL-C license.
! 
! CROCO website : http://www.croco-ocean.org
!======================================================================
!
/* 
 This is "cppdefs_dev.h": It contains a set of predetermined
 macro definitions which are inserted into the individual files by
 C-preprocessor. General user is discouraged from attempts
 to modify anything below this line. But developers are 
 encouraged to do so if needed 
*/

/* 
======================================================================
   Set debugging options
======================================================================
*/ 

/*  Switch to mixed [tiled + single-block] execution. Activation of
 this switch enables special logical branch in "compute_tile_bounds"
 which recognizes tile=NSUB_X*NSUB_E as covering the whole model
 grid, and it increases sizes of arrays declared in "private_scratch"
 to accomodate enough workspace accordingly. This switch is used for
 debugging purposes only and normally should be undefined.
*/
#undef ALLOW_SINGLE_BLOCK_MODE
#ifdef ALLOW_SINGLE_BLOCK_MODE
# define SINGLE NSUB_X*NSUB_E,NSUB_X*NSUB_E !!!
#endif

/*  
   Activate the RVTK_DEBUG procedure that will test the reproducibility 
   of parallel computation by comparing binary files produced by serial 
   and parallel runs
*/
/* !#undef RVTK_DEBUG */

/*
    Constant tracer option (for debugging)
*/
#undef CONST_TRACERS

/* 
======================================================================
   Set OA COUPLING options:
   Define MPI, select OA_MCT    
   Change the generic name of MPI communicator MPI_COMM_WORLD
   to OASIS-MCT local communicator
======================================================================
*/ 
#ifdef OA_COUPLING
# undef  OPENMP
# define MPI
# define OA_MCT
# define MPI_COMM_WORLD ocean_grid_comm
# undef  OA_GRID_UV
# undef  BULK_FLUX
# undef  QCORRECTION
# undef  SFLX_CORR
# undef  ANA_DIURNAL_SW
# undef  OA_GRID_UV
#endif

/* 
======================================================================
   Set OW COUPLING options:
   Define MPI, select OA_MCT    
   Change the generic name of MPI communicator MPI_COMM_WORLD
   to OASIS-MCT local communicator
======================================================================
*/
#ifdef OW_COUPLING
# undef  OPENMP
# define MPI
# define OA_MCT
# define MPI_COMM_WORLD ocean_grid_comm
# undef  WKB_WWAVE
# undef  WAVE_OFFLINE
# ifdef MRL_WCI
#  undef  WAVE_ROLLER
#  define WAVE_STREAMING
#  define WAVE_RAMP
# endif
#endif

/* 
======================================================================
   Set XIOS options:    
   Activate MPI
   Change the generic name of MPI communicator MPI_COMM_WORLD
   to XIOS local communicator
======================================================================
*/ 
#ifdef XIOS
# define MPI
# define MPI_COMM_WORLD ocean_grid_comm
# define key_iomput
#else
# undef key_iomput
#endif
  
/*
======================================================================
   Set default time-averaging filter for barotropic fields.
======================================================================
*/
#ifdef M2FILTER_NONE     /* Check if options are defined in cppdefs.h */
#elif defined M2FILTER_POWER
#elif defined M2FILTER_COSINE
#elif defined M2FILTER_FLAT
#else
# undef  M2FILTER_NONE
# define M2FILTER_POWER
# undef  M2FILTER_COSINE
# undef  M2FILTER_FLAT
#endif

/*
======================================================================
   Activate NBQ choices for non-hydrostatic simulations
======================================================================
*/
#ifdef NBQ              /* General options */
# define SOLVE3D
# define M2FILTER_NONE  /* no filter with NBQ */
# undef  M2FILTER_POWER
# define NBQ_IMP
# undef  NBQ_THETAIMP
# undef  NBQ_FREESLIP
# undef  NBQ_HZ_PROGNOSTIC
# ifdef  TANK
#  undef  NBQ_AM4
# else
#  define NBQ_AM4
# endif
# undef  TRACETXT
# undef  DIAG_CFL
# define HZR Hzr
/*
   NBQ Precise or Performance options (default: NBQ_PERF) 
*/
# ifndef NBQ_PRECISE
#  define NBQ_PERF
# endif
# ifdef NBQ_PERF
#  undef  NBQ_MASS
#  define NBQ_GRID_SLOW
#  define NBQ_HZCORRECT
# else
#  define NBQ_MASS
#  undef  NBQ_GRID_SLOW
#  define NBQ_HZCORRECT
# endif

/*
   Options for wz HADV numerical schemes (default C4)
*/
# ifdef W_HADV_SPLINES  /* Check if options are defined in cppdefs.h */
# elif defined W_HADV_TVD
# elif defined W_HADV_WENO5
# elif defined W_HADV_C4
# elif defined W_HADV_C2
# else
#  undef  W_HADV_SPLINES  /* Splines vertical advection             */
#  undef  W_HADV_TVD      /* TVD vertical advection                 */
#  undef  W_HADV_WENO5    /* 5th-order WENOZ vertical advection     */
#  define W_HADV_C4       /* 2nd-order centered vertical advection  */
#  undef  W_HADV_C2       /* 2nd-order centered vertical advection  */
# endif
/*
   Options for wz VADV numerical schemes (default SPLINES)
*/
# ifdef W_VADV_SPLINES  /* Check if options are defined in cppdefs.h */
# elif defined W_VADV_TVD
# elif defined W_VADV_WENO5
# elif defined W_VADV_C2
# else
#  define W_VADV_SPLINES  /* Splines vertical advection             */
#  undef  W_VADV_TVD      /* TVD vertical advection                 */
#  undef  W_VADV_WENO5    /* !!! 5th-order WENOZ vertical advection */
#  undef  W_VADV_C2       /* 2nd-order centered vertical advection  */
# endif
/*
   NBQ Open boundary conditions
*/
# if defined OBC_WEST  || defined OBC_EAST  || \
     defined OBC_NORTH || defined OBC_SOUTH
#  define OBC_NBQ
# endif
# ifdef OBC_NBQ          /* OBC options and nudging: default zero grad */
#  define OBC_NBQORLANSKI    /*  Radiative conditions           */
#  undef  OBC_NBQSPECIFIED   /*  Specified conditions (forcing) */
#  define NBQ_NUDGING        /* interior/bdy forcing/nudging    */
#  define NBQCLIMATOLOGY     /* interior/bdy forcing/nudging    */
#  undef  NBQ_FRC_BRY        /* bdy forcing/nudging             */
#  undef  W_FRC_BRY          /* wz bdy forcing/nudging          */
# endif

#else                /* Hydrostatic mode */

# define HZR Hz

#endif  /* NBQ */

/*
======================================================================
   Activate barotropic pressure gradient response to the
   perturbation of free-surface in the presence of stratification
======================================================================
*/
#if defined SOLVE3D
# define VAR_RHO_2D
# if !defined NONLIN_EOS && !defined INNERSHELF
#  define RESET_RHO0
# endif
#endif

/*
======================================================================
   Activate choice of Pressure Gradient formulation
   (default is the Density Jacobian formulation with Cubic 
   Polynomial fit from Shchepetkin et al. (2003). But:
   1- This code can be run cheaper for flat bottom cases if
      terms involving z-grid x/y gradients are removed
      (PGF_FLAT_BOTTOM)
   2- a cheaper standard Jacobian formulation can also be used 
   (PGF_BASIC_JACOBIAN) for smooth topography. 
   3- The Weighted Jacobian formulation of Song & Haidvogel (1994)
   can be used in this case by defining WJ_GRADP key, which then serves 
   as the weight value. 
======================================================================
*/
#if defined BASIN || defined EQUATOR  || defined GRAV_ADJ \
                  || defined SOLITON  || defined JET \
                  || defined ACOUSTIC || defined VORTEX \
                  || defined THACKER  || defined TANK \
                  || defined KH_INST
# define PGF_FLAT_BOTTOM
#elif defined RIP
# define PGF_BASIC_JACOBIAN
# define WJ_GRADP 0.125
#elif defined PGF_BASIC_JACOBIAN
# define WJ_GRADP 0.125
#endif

/*
======================================================================
    Activate EOS splitting of seawater compressibility effect in case 
    of non-linear formulation, as part of the pressure gradient
    algorithm with polynomial fit (Shchepetkin & McWilliams 2003)
======================================================================
*/
#ifdef NONLIN_EOS
# define SPLIT_EOS
#endif

/*
======================================================================
    Select MOMENTUM LATERAL advection-diffusion scheme:
    (The default is third-order upstream biased)
======================================================================
*/
#ifdef UV_HADV_UP3     /* Check if options are defined in cppdefs.h */
#elif defined UV_HADV_C4
#elif defined UV_HADV_C2
#elif defined UV_HADV_UP5
#elif defined UV_HADV_C6
#elif defined UV_HADV_WENO5
#elif defined UV_HADV_TVD
#else
# define UV_HADV_UP3       /* 3rd-order upstream lateral advection */
# undef  UV_HADV_C4        /* 4th-order centered lateral advection */
# undef  UV_HADV_C2        /* 2nd-order centered lateral advection */
# undef  UV_HADV_UP5	   /* 5th-order upstream lateral advection */
# undef  UV_HADV_C6	   /* 6th-order centered lateral advection */
# undef  UV_HADV_WENO5	   /* 5th-order WENOZ    lateral advection */
# undef  UV_HADV_TVD	   /*           TVD      lateral advection */
#endif
/* 
   UV DIFFUSION: set default orientation
*/
#ifdef UV_MIX_S        /* Check if options are defined */
#elif defined UV_MIX_GEO
#else
# define UV_MIX_S      /* Default: diffusion along sigma surfaces */
#endif
/* 
   Set keys related to Smagorinsky viscosity 
*/
#ifdef UV_VIS_SMAGO_3D
# define UV_VIS_SMAGO  
# define TS_DIF_SMAGO
#endif
#ifdef UV_VIS_SMAGO 
# define VIS_COEF_3D  
#endif
/*
   Set UP3 scheme in barotropic equations for 2DH applications
*/
#if !defined SOLVE3D && !defined SOLITON
# define M2_HADV_UP3
#endif
/*
   If interior MOMENTUM LATERAL diffusion is defined, apply it
   over an anomaly with respect to a reference frame (climatology)
*/
#ifdef M3CLIMATOLOGY
# undef CLIMAT_UV_MIXH
#endif

/*
======================================================================
    Select MOMENTUM VERTICAL advection scheme:
======================================================================
*/
#ifdef UV_VADV_SPLINES  /* Check if options are defined in cppdefs.h */
#elif defined UV_VADV_WENO5
#elif defined UV_VADV_C2
#elif defined UV_VADV_TVD
#else
# define UV_VADV_SPLINES   /*            Splines vertical advection  */
# undef  UV_VADV_WENO5     /* 5th-order  WENOZ   vertical advection  */
# undef  UV_VADV_C2        /* 2nd-order centered vertical advection  */
# undef  UV_VADV_TVD       /*            TVD     vertical advection  */
#endif

#ifdef VADV_ADAPT_IMP      /* Semi-implicit vertical advection       */
# undef  VADV_ADAPT_PRED   /* apply to both pred/corr steps (choice) */
# define UV_VADV_SPLINES   /* Impose splines advection (no choice)   */
# undef  UV_VADV_C2
#endif

/*
======================================================================
    Select TRACER LATERAL advection-diffusion scheme
    (The default is third-order upstream biased)
======================================================================
*/
#ifdef TS_HADV_UP3    /* Check if options are defined in cppdefs.h */
#elif defined TS_HADV_C4
#elif defined TS_HADV_UP5
#elif defined TS_HADV_WENO5
#elif defined TS_HADV_C6
#elif defined TS_HADV_RSUP3
#elif defined TS_HADV_RSUP5
#else
# define TS_HADV_UP3    /* 3rd-order upstream lateral advection */
# undef  TS_HADV_C4     /* 4th-order centered lateral advection */
# undef  TS_HADV_UP5    /* 5th-order upstream lateral advection */
# undef  TS_HADV_WENO5  /* 5th-order WENOZ    lateral advection */
# undef  TS_HADV_C6     /* 6th-order centered lateral advection */
# undef  TS_HADV_RSUP3  /* Rotated-Split UP3  lateral advection */
# undef  TS_HADV_RSUP5  /* Pseudo R-Split UP5 lateral advection */
#endif
/* 
  Options for split-rotated advection-diffusion schemes
*/
#ifdef TS_HADV_RSUP3   /*  Rotated-Split 3rd-order scheme is:  */
# define TS_HADV_C4    /*    4th-order centered advection      */
# undef  TS_DIF2       /*               +                      */
# define TS_DIF4       /*         Hyperdiffusion  with         */
# undef  TS_MIX_GEO    /*        Geopotential rotation         */
# define TS_MIX_ISO    /*     or Isopycnal    rotation         */
#  if defined GLS_MIX2017 || defined GLS_MIXING
#   undef  TS_MIX_ISO
#   define TS_MIX_GEO
#  endif
#endif
#ifdef TS_HADV_RSUP5   /*    Pseudo RS 5th-order scheme is:    */
# define TS_HADV_C6    /*    6th-order centered advection      */
# undef  TS_DIF2       /*               +                      */
# define TS_DIF4       /*         Hyperdiffusion  with         */
# define TS_MIX_GEO    /*        Geopotential rotation         */
# undef  TS_MIX_ISO    /*     or Isopycnal    rotation         */
#endif
#if defined TS_HADV_C4 && !defined TS_HADV_RSUP3     
                       /* 4th-order centered advection with:   */
# define TS_DIF2       /*   + Laplacian Diffusion              */
# undef  TS_DIF4       /*                                      */
# define TS_DIF_SMAGO  /*   + Smagorinsky diffusivity          */
# define TS_MIX_ISO    /*   + Isopycnal rotation               */ 
#endif 

/* 
   TS DIFFUSION: set default orientation
*/
#ifdef TS_MIX_S        /* Check if options are defined  */
#elif defined TS_MIX_GEO
#elif defined TS_MIX_ISO
#else
# define TS_MIX_S      /* Set iso-sigma diffusion as default */
#endif
/* 
   Apply implicit treatment and filters
*/
#if defined TS_MIX_ISO || (defined TS_DIF4 && defined TS_MIX_GEO)
# define TS_MIX_IMP       /*  Implicit treatment of vertical fluxes  */
#endif
#if defined TS_MIX_ISO && defined SALINITY
# define TS_MIX_ISO_FILT  /*  neutral slope filtering */
#endif
/*
   Apply interior diffusion (if defined) over tracer anomalies
   with respect to a reference frame (climatology)
*/
# ifdef TCLIMATOLOGY
#  undef CLIMAT_TS_MIXH
#  undef CLIMAT_TS_MIXH_FINE
# endif
/*
   Use 3D diffusivity arrays if needed       
*/
#if defined TS_HADV_RSUP3 \
 || defined TS_HADV_RSUP5 || defined TS_DIF_SMAGO
# define DIF_COEF_3D
#endif
/* 
   If BIO_HADV_WENO5 is chosen, the advection scheme for passive tracers is
   independent from that selected for the two active tracers (TS_HADV)
*/
#ifdef BIO_HADV_WENO5
# define NTRA_T3DMIX 2    /* TS_HADV applied over the 2 active tracers */
#else
# define NTRA_T3DMIX NT   /* TS_HADV applied over all NT tracers       */
#endif

/*
======================================================================
    Select model dynamics for TRACER vertical advection
    (The default is 4th-order centered)
======================================================================
*/
#ifdef TS_VADV_SPLINES  /* Check if options are defined in cppdefs.h */
#elif defined TS_VADV_AKIMA
#elif defined TS_VADV_WENO5
#elif defined TS_VADV_C2
#else
# undef  TS_VADV_SPLINES   /* Splines vertical advection            */
# define TS_VADV_AKIMA     /* 4th-order Akima vertical advection    */
# undef  TS_VADV_WENO5     /* 5th-order WENOZ vertical advection    */
# undef  TS_VADV_C2        /* 2nd-order centered vertical advection */
#endif

#undef  TS_VADV_FCT        /* Flux correction of vertical advection */

#ifdef VADV_ADAPT_IMP
# define  TS_VADV_SPLINES
# undef   TS_VADV_AKIMA
# undef   TS_VADV_WENO5
# undef   TS_VADV_C2
#endif
/*
======================================================================
   SPONGE:  
   define SPONGE_GRID, SPONGE_DIF2 and SPONGE_VIS2 
======================================================================
*/
#ifdef SPONGE
# ifndef INNERSHELF 
#  define SPONGE_GRID
# endif
# define SPONGE_DIF2
# define SPONGE_VIS2
# ifdef SEDIMENT
#  define SPONGE_SED
# endif
#endif

/*
======================================================================
   TIDES:  
   select dependable keys if not done yet
======================================================================
*/
#ifdef TIDES
# ifdef SSH_TIDES
#  ifdef ZCLIMATOLOGY
#  elif defined Z_FRC_BRY
#  else
#   define ZCLIMATOLOGY
#   define ANA_SSH
#  endif
# endif
# ifdef UV_TIDES
#  ifdef M2CLIMATOLOGY
#  elif defined M2_FRC_BRY
#  else
#   define M2CLIMATOLOGY
#   define ANA_M2CLIMA
#  endif
# endif
#endif

/*
======================================================================
    PSOURCE / PSOURCE_NCFILE option
======================================================================
*/
#if defined PSOURCE
#  define ANA_PSOURCE  /* ON: set vertical profil for qbar */
#endif
#if defined PSOURCE_NCFILE
# define PSOURCE
# define ANA_PSOURCE
#endif

/*
======================================================================
    Bulk flux option
======================================================================
*/
#ifdef BULK_FLUX
# ifdef BULK_SMFLUX     
#  define BULK_SM_UPDATE /* ON: Compute wind stress via bulk_flux */
# endif
# ifdef ONLINE
#  define CUBIC_INTERP
# endif
#endif

/*
======================================================================
    Wave Current Interaction
======================================================================
*/

#if defined MRL_WCI || defined WKB_WWAVE
/*  Wave breaking dissipation (both WKB and WCI) */
# undef  WAVE_SFC_BREAK
# ifdef WAVE_BREAK_CT93
# elif  WAVE_BREAK_TG86
# elif  WAVE_BREAK_TG86A
# elif  WAVE_BREAK_R93
# else
#  define WAVE_BREAK_CT93 /* defaults */
# endif
#endif

/* WKB specific options  */
#ifdef WKB_WWAVE
# ifdef MRL_CEW
#  undef  WKB_KZ_FILTER
#  undef  WKB_TIME_FILTER
# endif
# define WKB_ADD_DIFF
# undef  WKB_ADD_DIFFRACTION
# undef  WKB_NUDGING
# ifndef WAVE_OFFLINE
#  undef WKB_NUDGING
# endif
# if defined SHOREFACE || defined FLUME \
                       || (defined RIP && !defined BISCA)
#  define ANA_BRY_WKB
# endif
#endif

#ifdef MRL_WCI
/* Bottom streaming */
# ifdef WAVE_STREAMING
#  define WAVE_BODY_STREAMING
# endif
/* Default WCI is with input file data (WAVE_OFFLINE)  */
# if !defined WKB_WWAVE && !defined ANA_WWAVE && !defined OW_COUPLING
#  define WAVE_OFFLINE
#  undef  WAVE_ROLLER
# endif
#endif

# if defined WKB_WWAVE || defined OW_COUPLING || defined WAVE_OFFLINE
#  define WAVE_IO
# endif

/*
======================================================================
                           KPP options
                      IF-less KPP --> KPP2005
======================================================================
*/
#ifdef LMD_SKPP
# define LMD_SKPP2005
#endif
#ifdef LMD_BKPP
# undef LMD_BKPP2005  /*<- unresolved problems with bkpp2005 at depth 
                           default: lmd_bkpp1994 */
#endif

/*
======================================================================
                Biogeochemical models
======================================================================
*/
#ifdef BIOLOGY
# ifdef PISCES
#  undef DIURNAL_INPUT_SFLX    /* Under Development */
#  define key_trc_pisces
#  define key_passivetrc
#  ifdef DIAGNOSTICS_BIO
#   define key_trc_diaadd
#   define key_trc_dia3d
#  endif
# endif
# ifdef BIO_BioEBUS
#  undef VAR_CHL_C             /* Under Development */
#  undef CARBON                /* Under Development */
#  undef HYDROGEN_SULFIDE      /* Under Development */
# endif
#endif

/*
======================================================================
    Bottom stress option:

    LIMIT_BSTRESS: Set limiting factor for bottom stress and avoid 
    numerical instability associated with reversing bottom flow
======================================================================
*/
#ifndef INNERSHELF
# define LIMIT_BSTRESS
#endif
#ifdef BBL
# ifdef OW_COUPLING
# elif defined WAVE_OFFLINE
# elif defined WKB_WWAVE
# else
#  define ANA_WWAVE
# endif
# ifdef SEDIMENT
#  undef  ANA_BSEDIM
# else
#  define ANA_BSEDIM
# endif
# ifdef SEDIMENT
#  define Z0_BL
# else
#  undef  Z0_BL
# endif
# ifdef Z0_BL
#  define Z0_RIP
# endif
# undef  Z0_BIO
#endif

/*
======================================================================
                Sediment dynamics models
======================================================================
*/
#ifdef SEDIMENT
# define SUSPLOAD
# define BEDLOAD
# ifdef BEDLOAD
#  undef  SLOPE_NEMETH
#  define SLOPE_LESSER
#  if (defined WAVE_OFFLINE || defined WKB_WWAVE || defined ANA_WWAVE\
                            || defined OW_COUPLING)
#   define BEDLOAD_SOULSBY
#   define Z0_BL  /* Mandatory with BEDLOAD_SOULSBY */
#   define Z0_RIP
#  else
#   define BEDLOAD_MPM
#  endif
# endif
/* 
     Morphodynamics (bed evolution feedback on circulation):
     MORPHODYN or MOVING_BATHY (equivalent) must be defined
     in cppdefs.h (default is undefined)
*/
# if defined MORPHODYN || defined MOVING_BATHY
#  ifdef MOVING_BATHY
#  else
#   define MOVING_BATHY
#  endif
# else
#  undef  MOVING_BATHY
# endif
#endif /* SEDIMENT */
/*
======================================================================
                              OBCs
======================================================================
*/
#if defined OBC_EAST || defined OBC_WEST || defined OBC_SOUTH\
                     || defined OBC_NORTH

# ifdef OBC_M2SPECIFIED
# elif defined OBC_M2FLATHER
# elif defined OBC_M2CHARACT
# elif defined OBC_M2ORLANSKI
# else
#  undef  OBC_M2SPECIFIED
#  undef  OBC_M2FLATHER
#  define OBC_M2CHARACT
#  undef  OBC_M2ORLANSKI
# endif
# ifdef  OBC_M2ORLANSKI
#  define OBC_VOLCONS
# endif

# ifdef SOLVE3D

#  ifdef OBC_M3SPECIFIED
#  elif defined OBC_M3ORLANSKI
#  else
#   undef  OBC_M3SPECIFIED
#   define OBC_M3ORLANSKI
#  endif

#  ifdef OBC_TSPECIFIED
#  elif defined OBC_TORLANSKI
#  else
#   undef  OBC_TSPECIFIED
#   define OBC_TORLANSKI
#  endif

# endif /* SOLVE3D */

#endif /* OBC */

/*
======================================================================
                       AGRIF nesting
======================================================================
*/
#ifdef AGRIF
#define key_agrif
/*                    Update schemes */
# undef  AGRIF_UPDATE_MIX_LOW
# define AGRIF_UPDATE_MIX
# undef  AGRIF_UPDATE_DECAL
/*                    Conservation options */
# define AGRIF_CONSERV_VOL
# undef  AGRIF_CONSERV_TRA
/*                    Sponge layer */
# define SPONGE
# define SPONGE_DIF2
# define SPONGE_VIS2
/*                    Boundary conditions */
# define AGRIF_OBC_EAST
# define AGRIF_OBC_WEST
# define AGRIF_OBC_NORTH
# define AGRIF_OBC_SOUTH

# define AGRIF_FLUX_BC 

# define AGRIF_OBC_M2SPECIFIED
# ifdef AGRIF_2WAY
#  define AGRIF_OBC_M3SPECIFIED
#  define AGRIF_OBC_TSPECIFIED
# else
#  define AGRIF_OBC_M3ORLANSKI
#  define AGRIF_OBC_TORLANSKI
# endif
#endif /* AGRIF */

/*
======================================================================
                            Standard I/O
======================================================================

   Set land mask value to _FillValue
*/ 
#undef  FILLVAL

/* 
  Write start_date information in netCDF output
  (in roms.in, add the keyword start_date:
  For example, if the simulation starts 1 January of 2000, at 00:00:00
  start_date: 01-JAN-2000 00:00:00) 
*/ 
#undef  START_DATE

/* 
  Define the NetCDF creation mode flag:
  nf_clobber (classic), nf_64bit_offset (large files) or nf_netcdf4
*/ 
#define NF_CLOBBER nf_64bit_offset

/*
======================================================================

                  Consistency for 2D configurations

======================================================================
*/
#ifndef SOLVE3D
# undef AVERAGES_K
# undef SALINITY
# undef NONLIN_EOS
# undef SPLIT_EOS
# undef QCORRECTION
# undef SFLX_CORR
# undef ANA_DIURNAL_SW
# undef ANA_STFLUX
# undef ANA_SSFLUX
# undef ANA_SRFLUX
# undef BULK_FLUX
# undef TS_DIF2
# undef TS_DIF4
# undef CLIMAT_TS_MIXH
# undef SPONGE_DIF2
# undef TS_HADV_RSUP3
# undef TS_MIX_GEO
# undef TS_MIX_ISO
# undef TS_DIF_SMAGO
# undef UV_MIX_GEO
# undef VIS_COEF_3D
# undef DIF_COEF_3D
# undef M3NUDGING
# undef TNUDGING
# undef ROBUST_DIAG
# undef M3CLIMATOLOGY
# undef TCLIMATOLOGY
# undef M3_FRC_BRY
# undef T_FRC_BRY
# undef BODYFORCE
# undef BVF_MIXING
# undef LMD_MIXING
# undef LMD_BKPP
# undef LMD_SKPP
# undef LMD_RIMIX
# undef LMD_CONVEC
# undef OBC_M3ORLANSKI
# undef OBC_M3SPECIFIED
# undef OBC_TORLANSKI
# undef OBC_TSPECIFIED
# undef AGRIF_OBC_M3ORLANSKI
# undef AGRIF_OBC_M3SPECIFIED
# undef AGRIF_OBC_TORLANSKI
# undef AGRIF_OBC_TSPECIFIED
# undef SEDIMENT
# undef BIOLOGY
#endif


! $Id: debug.F 1458 2014-02-03 15:01:25Z gcambon $
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
#include "cppdefs.h"
#if defined RVTK_DEBUG || defined RVTK_DEBUG_ADVANCED

          module debug
          integer nbprocs_in
          integer,dimension(:),allocatable :: debug_procs_units
          end module debug
!
!======================================================================
!                    debug_ini
!======================================================================
!
          subroutine debug_ini()
          USE debug
#include "param.h"
#include "scalars.h"
          character(len=80) :: filename
          character(len=80) :: tmp_string
          integer :: nb
          
          filename="check_file"
#if !defined RVTK_DEBUG_WRITE
          open(107,file="debug_infos",form="formatted",
     &  status='OLD')
            read(107,*)nbprocs_in
          close(107)
          allocate(debug_procs_units(0:nbprocs_in-1))
          do nb = 0,nbprocs_in-1
             write(filename,'(a11,i0,a1,i0)')
     &    "check_file_",nbprocs_in,'_',nb
            open(newunit=debug_procs_units(nb),
     &           file=filename,form='unformatted',
     &           status='OLD')
          enddo
#else
#if defined MPI
          if (mynode == 0) then
          open(107,file="debug_infos",form="formatted")
            write(107,*)NNODES
          close(107)
          endif
          write(filename,'(a11,i0,a1,i0)')
     &    "check_file_",NNODES,'_',mynode
#else
          open(107,file="debug_infos",form="formatted")
            write(107,*)1
          close(107)
          write(filename,'(a10')"check_file_1_0"
#endif
          open(107,file=filename,form='unformatted',
     &            status='UNKNOWN')
#endif
          return

          end subroutine debug_ini
!
!======================================================================
!                    check_tab2d
!======================================================================
!
          subroutine check_tab2d(tab,comment,typevar)
      USE debug
      implicit none
# include "param.h"
# include "scalars.h"  
# include "ocean2d.h" 
# include "mpi_roms.h"
# ifdef MPI
      include 'mpif.h'
# endif   
      real,dimension(GLOBAL_2D_ARRAY) :: tab
      integer Lmseq,Mmseq
      integer lb(2),ub(2)
      real,dimension(:,:),allocatable :: tabread
      character*(*) :: comment,typevar
      integer i0,j0,i,j,i1,j1,i2,j2
#if !defined RVTK_DEBUG_WRITE
      integer i1_r,i2_r,j1_r,j2_r,nb
#endif
# ifdef OPENMP
#  undef mynode
      integer mynode 
# endif         
      integer iseq1, iseq2, jseq1, jseq2
      logical mystop
# ifdef MPI
      logical globstop
      integer ierr
# endif
          
# if defined OPENMP && !defined MPI
      mynode=0 
# endif

!# if !defined MPI && !defined OPENMP
#if defined RVTK_DEBUG_WRITE
#include "debug_indices.h"
#if defined MPI
      i1 = i1 + iminmpi - 1
      i2 = i2 + iminmpi - 1
      j1 = j1 + jminmpi - 1
      j2 = j2 + jminmpi - 1
#endif
      write(107)i1,i2,j1,j2
      write(107)lbound(tab)
#if defined MPI
     &  + (/iminmpi-1,jminmpi-1/)
#endif
      write(107)ubound(tab)
#if defined MPI
     &  + (/iminmpi-1,jminmpi-1/)
#endif
      write(107)tab
# else
      do nb = 0, nbprocs_in-1
      read(debug_procs_units(nb))i1_r,i2_r,j1_r,j2_r
      read(debug_procs_units(nb))lb
      read(debug_procs_units(nb))ub
      allocate(tabread(lb(1):ub(1),lb(2):ub(2)))
      read(debug_procs_units(nb))tabread

#include "debug_indices.h"
      
      iseq1 = i1_r
      iseq2 = i2_r
      jseq1 = j1_r
      jseq2 = j2_r


      mystop =.false.
      DebugExit : do j=max(j1,jseq1-jminmpi+1),min(j2,jseq2-jminmpi+1)
                  j0 = j+jminmpi-1
                  do i=max(i1,iseq1-iminmpi+1),min(i2,iseq2-iminmpi+1)
                    i0 = i+iminmpi-1
                    if (tabread(i0,j0)/=tab(i,j)) then
                       write(*,'(A,A,2x,5i4,3e20.12)')'BUGBIN = ',
     &                 comment,mynode,i0,j0,i,j,
     &                 tabread(i0,j0),tab(i,j),
     &                 abs(tabread(i0,j0)-tab(i,j))
!                       print *,'iif = ',iif,iic
#  ifdef AGRIF
                       print *,'GRID# ',Agrif_CFixed()
#  endif                    
                       mystop=.true.
                       exit DebugExit
                     endif
                   enddo
                 enddo DebugExit
#  ifdef MPI         
      call MPI_allreduce(mystop,globstop,1,
     &                   MPI_LOGICAL,MPI_LOR,MPI_COMM_WORLD,ierr)
      mystop=globstop
#  endif
      if (mystop) then
        stop
      endif
            
      deallocate(tabread)
      enddo /* do nb = 0, nbprocs_in-1 */
# endif /* !defined RVTK_DEBUG_WRITE */      

# ifdef MPI
      call MPI_Barrier(MPI_COMM_WORLD,ierr)
# endif

#if !defined RVTK_DEBUG_WRITE
      MPI_master_only print *,'CHECK ',comment,' PASSED'
#  ifdef AGRIF
     &                       ,' ON GRID ',Agrif_CFixed()
#  endif
# endif

          return
          end subroutine check_tab2d
!
!======================================================================
!                    check_tab3d
!======================================================================
!
          subroutine check_tab3d(tab,comment,typevar)
      USE debug
      implicit none
# include "param.h"
# include "scalars.h"  
# include "ocean2d.h" 
# ifdef MPI
      include 'mpif.h'
# endif   
# ifdef MPI
      logical globstop
      integer ierr
# endif 
      real,dimension(GLOBAL_2D_ARRAY,N) :: tab
      integer Lmseq,Mmseq
      integer lb(3),ub(3)
      real,dimension(:,:,:),allocatable :: tabread
      character*(*) :: comment,typevar
      integer i0,j0,k,i,j,i1,i2,j1,j2
#if !defined RVTK_DEBUG_WRITE
      integer i1_r,i2_r,j1_r,j2_r,nb
#endif
      character*80 :: comment_k
# ifdef OPENMP
      integer mynode
# endif
      integer iseq1, iseq2, jseq1, jseq2 
      logical mystop     

      if (typevar == 'w') then
        do k=1,N+1
          write(comment_k,'(a,a,i0)')TRIM(comment),' layer : ',k-1
          call check_tab2d(tab(START_2D_ARRAY,k),
     &                     TRIM(comment_k),'r')
        enddo
        return
      elseif (typevar == 'wint') then
        do k=1,N+1
          write(comment_k,'(a,a,i0)')TRIM(comment),' layer : ',k-1
          call check_tab2d(tab(START_2D_ARRAY,k),
     &                         TRIM(comment_k),'rint')
        enddo
        return
      endif        
          
# if defined OPENMP && !defined MPI
          mynode=0 
# endif

#if defined RVTK_DEBUG_WRITE
#include "debug_indices.h"
#if defined MPI
      i1 = i1 + iminmpi - 1
      i2 = i2 + iminmpi - 1
      j1 = j1 + jminmpi - 1
      j2 = j2 + jminmpi - 1
#endif
      write(107)i1,i2,j1,j2
      write(107)lbound(tab)
#if defined MPI
     &  + (/iminmpi-1,jminmpi-1,0/)
#endif
      write(107)ubound(tab)
#if defined MPI
     &  + (/iminmpi-1,jminmpi-1,0/)
#endif
      write(107)tab
# else
      
      do nb = 0, nbprocs_in-1
      read(debug_procs_units(nb))i1_r,i2_r,j1_r,j2_r
      read(debug_procs_units(nb))lb
      read(debug_procs_units(nb))ub
      allocate(tabread(lb(1):ub(1),lb(2):ub(2),lb(3):ub(3)))
      read(debug_procs_units(nb))tabread

#include "debug_indices.h"

      iseq1 = i1_r
      iseq2 = i2_r
      jseq1 = j1_r
      jseq2 = j2_r
      
      mystop =.false.
      DebugExit : do k=1,N
                do j=max(j1,jseq1-jminmpi+1),min(j2,jseq2-jminmpi+1)
                 j0 = j+jminmpi-1
                do i=max(i1,iseq1-iminmpi+1),min(i2,iseq2-iminmpi+1)
                    i0 = i+iminmpi-1
                     if (tabread(i0,j0,k)/=tab(i,j,k)) then
                       write(*,'(A,A,2x,6i4,3e20.12)')'BUGBIN = ',
     &                 comment,
     &                 mynode,i0,j0,i,j,k,
     &                 tabread(i0,j0,k),tab(i,j,k),
     &                 abs(tabread(i0,j0,k)-tab(i,j,k))
!                      print *,'iif = ',iif,iic
#  ifdef AGRIF
                       print *,'GRID# ',Agrif_CFixed()
#  endif    
                        mystop=.true.
                       exit DebugExit
                      endif
                     enddo
                    enddo
                   enddo DebugExit
#  ifdef MPI   
      call MPI_allreduce(mystop,globstop,1,
     &                   MPI_LOGICAL,MPI_LOR,MPI_COMM_WORLD,ierr)
      mystop=globstop
#  endif
      if (mystop) then
        stop
      endif
            
      deallocate(tabread)
      enddo /* do nb = 0, nbprocs_in-1 */
# endif /* !defined RVTK_DEBUG_WRITE */

# ifdef MPI
      call MPI_Barrier(MPI_COMM_WORLD,ierr)
# endif

#if !defined RVTK_DEBUG_WRITE
      MPI_master_only print *,'CHECK ',comment,' PASSED'
#  ifdef AGRIF
     &                       ,' ON GRID ',Agrif_CFixed()
#  endif
# endif
          return
          end subroutine check_tab3d    
          
#else /*  RVTK_DEBUG */

        subroutine debug_empty()
        return
        end subroutine debug_empty
#endif                

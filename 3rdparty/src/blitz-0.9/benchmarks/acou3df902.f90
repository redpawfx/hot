!
! F90 Tuned version
! Optimizations:
! - One 4D array allocated, other arrays are slices of this array.  This
!   arrangement interlaces the arrays in memory, improving data locality
! - Rather than copying arrays, the indices into the 4D array are copied
! - Rely on compiler for tiling.

SUBROUTINE acoustic3d_f90Tuned(N, niters, check)
  INTEGER, INTENT( IN ) :: N, niters
  REAL, INTENT( OUT ) :: check
  REAL, DIMENSION (N,N,N) :: P1, P2, P3, C
  INTEGER iter

  CALL acoustic3d_f90Tuned_setup(P1, P2, P3, c, N)

  IF (MODULO(niters,3) > 0) THEN
    PRINT *, 'Warning: In acoustic3d_f90Tuned: niters mod 3 != 0'
    PRINT *, 'Will do fewer iterations, benchmark result will be off'
  ENDIF

  DO iter=1, niters, 3
    CALL acoustic3d_f90Tuned_stencil(P1, P2, P3, C, N)
    CALL acoustic3d_f90Tuned_stencil(P2, P3, P1, C, N)
    CALL acoustic3d_f90Tuned_stencil(P3, P1, P2, C, N)
  END DO
    
  check = P1(N/2,N/2,N/2)

  RETURN
END

SUBROUTINE acoustic3d_f90Tuned_stencil(P1, P2, P3, C, N)
  INTEGER, INTENT( IN ) :: N
  REAL, DIMENSION (N,N,N), INTENT( IN ) :: P1, P2, C
  REAL, DIMENSION (N,N,N), INTENT( OUT ) :: P3

  P3(2:N-1,2:N-1,2:N-1) = (2-6*C(2:N-1,2:N-1,2:N-1)) &
        * P2(2:N-1,2:N-1,2:N-1) + C(2:N-1,2:N-1,2:N-1) &
        * (P2(1:N-2,2:N-1,2:N-1) + P2(3:N,2:N-1,2:N-1) &
        + P2(2:N-1,1:N-2,2:N-1) + P2(2:N-1,3:N,2:N-1) &
        + P2(2:N-1,2:N-1,1:N-2)+P2(2:N-1,2:N-1,3:N)) &
        - P1(2:N-1,2:N-1,2:N-1)
  RETURN
END


SUBROUTINE acoustic3d_f90Tuned_setup(P1, P2, P3, c, N)
  INTEGER, INTENT( IN ) :: N
  REAL, DIMENSION (N,N,N), INTENT( INOUT ) :: P1, P2, P3, c

  INTEGER cavityLeft, cavityRight, cavityFront, cavityBack, cavityTop, &
          cavityBottom, cavityTop2, cavityBottom2
  REAL ci, cj, ck, s2

! Set the velocity field

  c(1:N/2,:,:) = 0.05
  c(N/2+1:N,:,:) = 0.3;

  cavityLeft   = 3 * N / 7.0
  cavityRight  = 4 * N / 7.0
  cavityFront  = 3 * N / 7.0
  cavityBack   = 4 * N / 7.0
  cavityTop    = 5 * N / 7.0
  cavityBottom = 6 * N / 7.0
  cavityTop2   = 1 * N / 7.0
  cavityBottom2 = 2 * N / 7.0

  c(cavityTop:cavityBottom,cavityLeft:cavityRight, &
    cavityFront:cavityBack) = 0.02;
  c(cavityTop2:cavityBottom2,cavityLeft:cavityRight, &
    cavityFront:cavityBack) = 0.001;

! Initial pressure distribution
  P1 = 0.0
  P3 = 0.0

  ci = N/2.0
  cj = N/2.0
  ck = N/2.0
  s2 = 64.0 * 9.0 / ((N/2.0) ** 2)

  DO k=1,N
    DO j=1,N
      DO i=1,N
        P2(i,j,k) = exp(-((i-ci)**2+(j-cj)**2+(k-ck)**2) * s2)
      END DO
    END DO
  END DO

  CALL acoustic3d_f90Tuned_setup_check(P2, N)
  CALL acoustic3d_f90Tuned_setup_check(c, N)

  RETURN
END


SUBROUTINE acoustic3d_f90Tuned_setup_check(P, N)
  INTEGER, INTENT( IN ) :: N
  REAL, INTENT( IN ), DIMENSION(N,N,N) :: P

  INTEGER i, j, k
  REAL check

  check = 0.0
  DO k=1,N
    DO j=1,N
      DO i=1,N
        check = check + P(i,j,k) * (i+N*j+N*N*k)
      END DO
    END DO
  END DO

  PRINT *, 'Array check: ', check

  RETURN
END


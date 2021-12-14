#include <stdio.h>

void vec_dgemm_opt_c(int m_, int n_, int k_, double * result, double * A, double * B)
{
    asm volatile ("vsetcfg 9, 1");

    int vlen_result;
    asm volatile ("vsetvl %0, %1" : "=r" (vlen_result) : "r" (k_));

    void * vpset_vfblockaddr;
    asm volatile ("la %0, dgemm_opt_v_4_4_vpset" : "=r" (vpset_vfblockaddr));
    asm volatile ("vf 0(%0)" : : "r" (vpset_vfblockaddr));

    void * pre_vfblockaddr;
    void * main_vfblockaddr;
    void * post_vfblockaddr;
    asm volatile ("la %0, dgemm_opt_v_4_4_pre" : "=r" (pre_vfblockaddr));
    asm volatile ("la %0, dgemm_opt_v_4_4" : "=r" (main_vfblockaddr));
    asm volatile ("la %0, dgemm_opt_v_4_4_post" : "=r" (post_vfblockaddr));

    for (int i = 0; i < m_; i+=4) {
        for (int k = 0; k < k_; ) {
            int consumed;
#ifdef ARTIFICAL_LIMIT
            int artificial = k_-k < ARTIFICAL_LIMIT ? k_-k : ARTIFICAL_LIMIT;
#else
            int artificial = k_-k;
#endif

            asm volatile ("vsetvl %0, %1" : "=r" (consumed) : "r" (artificial));

            // C rows 1, 2, 3, 4
            asm volatile ("vmca va0, %0" : : "r" (&result[i*k_+k]));
            asm volatile ("vmca va1, %0" : : "r" (&result[(i+1)*k_+k]));
            asm volatile ("vmca va2, %0" : : "r" (&result[(i+2)*k_+k]));
            asm volatile ("vmca va3, %0" : : "r" (&result[(i+3)*k_+k]));

            asm volatile ("vf 0(%0)" : : "r" (pre_vfblockaddr));

            for (int j = 0; j < n_; j+=4) {

                // B row 1, 2, 3, 4
                asm volatile ("vmca va4, %0" : : "r" (&B[j*k_+k]));
                asm volatile ("vmca va5, %0" : : "r" (&B[(j+1)*k_+k]));
                asm volatile ("vmca va6, %0" : : "r" (&B[(j+2)*k_+k]));
                asm volatile ("vmca va7, %0" : : "r" (&B[(j+3)*k_+k]));

                // A row 1, 2, 3, 4
                asm volatile ("vmcs vs1, %0\n"
                              "vmcs vs2, %1\n"
                              "vmcs vs3, %2\n"
                              "vmcs vs4, %3\n"

                              "vmcs vs5, %4\n"
                              "vmcs vs6, %5\n"
                              "vmcs vs7, %6\n"
                              "vmcs vs8, %7\n"

                              "vmcs vs9, %8\n"
                              "vmcs vs10, %9\n"
                              "vmcs vs11, %10\n"
                              "vmcs vs12, %11\n"

                              "vmcs vs13, %12\n"
                              "vmcs vs14, %13\n"
                              "vmcs vs15, %14\n"
                              "vmcs vs16, %15"
                        : 
                        : "r" (A[j+(i+0)*n_+0]), "r" (A[j+(i+0)*n_+1]), "r" (A[j+(i+0)*n_+2]), "r" (A[j+(i+0)*n_+3]), 
                          "r" (A[j+(i+1)*n_+0]), "r" (A[j+(i+1)*n_+1]), "r" (A[j+(i+1)*n_+2]), "r" (A[j+(i+1)*n_+3]),
                          "r" (A[j+(i+2)*n_+0]), "r" (A[j+(i+2)*n_+1]), "r" (A[j+(i+2)*n_+2]), "r" (A[j+(i+2)*n_+3]),
                          "r" (A[j+(i+3)*n_+0]), "r" (A[j+(i+3)*n_+1]), "r" (A[j+(i+3)*n_+2]), "r" (A[j+(i+3)*n_+3])
                        );
                        
                asm volatile ("vf 0(%0)" : : "r" (main_vfblockaddr));
            }
            
            asm volatile ("vf 0(%0)" : : "r" (post_vfblockaddr));
            k += consumed;
        }
    }
    asm volatile ("fence");
}

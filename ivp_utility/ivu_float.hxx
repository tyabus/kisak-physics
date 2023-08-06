#ifdef WIN32
#include <float.h>
#endif

#if defined(POSIX) || defined(WIN32)
union p_double_ieee {
	IVP_DOUBLE val;
	struct {
		int val;
		unsigned int valh:20;
		unsigned int exp:11;
		unsigned int signum:1;
	} ln;
  struct {
    int l;
    int h;
  } ln2;
};
#define IVP_EXP_FOR_ONE 0x3ff
inline int PFM_LD(double a){ return ((p_double_ieee *)&(a))->ln.exp - IVP_EXP_FOR_ONE; };
#	endif

#	if defined(SUN) || defined(SUN4) || defined(__POWERPC__) || defined(GEKKO) 
union p_double_ieee {
	double val;
	struct {
		unsigned int signum:1;
		unsigned int exp:11;
		unsigned int valh:20;
		int val;
	} ln;
        struct {
		int h;
		int l;
	} ln2;
};

#	define P_EXP_FOR_ONE 0x3ff
	inline int PFM_LD(double a){ return ((p_double_ieee *)&(a))->ln.exp - P_EXP_FOR_ONE; };
#	endif

class IVP_Fast_Math {
public:
  // fast 1/sqrt(x),
  // resolution for resolution_steps
  // 0 -> 1e-3
  // 1 -> 1e-7
  // 2 -> 1e-14
  // 3 -> 1e-16
    static double isqrt(double square, int resolution_steps){
	p_double_ieee *ie = (p_double_ieee *)&square;
	IVP_ASSERT(IVP_Inline_Math::fabsd(square) > 0.0f);
	p_double_ieee h; h.val = 1.0f;
	h.ln2.h = ((0x07ff00000 - ie->ln2.h) >>1 ) + 0x1ff00000;
	IVP_DOUBLE squareh = square * 0.5f;
	IVP_DOUBLE inv_sqrt = h.val;
	
	inv_sqrt += inv_sqrt * (0.5f - inv_sqrt * inv_sqrt * squareh);
	inv_sqrt += inv_sqrt * (0.5f - inv_sqrt * inv_sqrt * squareh);
	if (resolution_steps > 0)	inv_sqrt += inv_sqrt * (0.5f - ( inv_sqrt * inv_sqrt * squareh ));
	if (resolution_steps > 1)	inv_sqrt += inv_sqrt * (0.5f - ( inv_sqrt * inv_sqrt * squareh ));
	if (resolution_steps > 2)	inv_sqrt += inv_sqrt * (0.5f - ( inv_sqrt * inv_sqrt * squareh ));

	IVP_ASSERT( IVP_Inline_Math::fabsd( 1.0f - inv_sqrt * inv_sqrt * square) < 0.001f );
	return inv_sqrt;
    }
    static IVP_DOUBLE sqrt(IVP_DOUBLE x) {
	return ::sqrt(x);
    }
};

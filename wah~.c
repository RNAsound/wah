/* wah~.c
 * by Aren Akian
 * MUS 177, FA 18
 *
 * Tom Erbe's svf~ filter, but modified the output to
 * create a wah filter.
 * This is done by adding the output of the bandpass to the
 * input signal, as shown in section 2.4.1 of DAFX by Udo Zolzer.
 *
 * Left inlet - controls BP central frequency
 * Right inlet - controls quality (limited to range [0, .9] for qualitative purposes )
 */


#include "m_pd.h"
#include <stdlib.h>
#include <math.h>
#ifdef NT
#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )
#endif

/* ------------------------ wah~ ----------------------------- */


static t_class *wah_class;

typedef struct _wah
{
    t_object x_obj; 	/* obligatory header */
    t_float frequency;    	/* floats on the first inlet set the frequency */
    t_float oldfrequency;
	t_float	quality;	/* the filter q or quality */
	t_float sample_rate;
	t_float notchOut, lowOut, highOut, bandOut, xout;
} t_wah;

    /* this is the actual performance routine which acts on the samples.
    It's called with a single pointer "w" which is our location in the
    DSP call list.  We return a new "w" which will point to the next item
    after us.  Meanwhile, w[0] is just a pointer to dsp-perform itself
    (no use to us), w[1] and w[2] are the input and output vector locations,
    and w[3] is the number of points to calculate. */
static t_int *wah_perform(t_int *w)
{
	t_wah *x = (t_wah *)(w[1]);
    t_float *in = (t_float *)(w[2]);
    t_float *out = (t_float *)(w[3]);
    int n = (int)(w[4]);

	// i like counting from zero, so i use sample to count the offset from
	// the start of the in and out blocks
	int sample = 0;
	float c, d, input;

    c = 2.0f * sin(3.141592653589793f * x->frequency/x->sample_rate);
    d = 2.0f * (1.0f - pow(x->quality, 0.25f));
    
    if(c > 0.5f) c = 0.5f;
    if(d > 2.0f) d = 2.0f;
    if(d > (2.0f/c - (c * 0.5f)))
        d = 2.0f/c - (c * 0.5f);

    while(n--)
    {
        input = *(in+sample);
        x->notchOut = input - (d * x->bandOut);
        x->lowOut   = x->lowOut + (c * x->bandOut);
        x->highOut  = x->notchOut - x->lowOut;
        x->bandOut  = x->bandOut + (c * x->highOut);
        
        /* wah routine */
        x->xout = (input - x->bandOut) + (c * x->bandOut);// out = input + BP
        *(out+sample) = (1.5f * x->xout) -
                        (0.5f * (x->xout * x->xout * x->xout));// polynomial approx. clipping function
        
		sample++;
    }
    return (w+5);
}

    /* called to start DSP.  Here we call Pd back to add our perform
    routine to a linear callback list which Pd in turn calls to grind
    out the samples. */
static void wah_dsp(t_wah *x, t_signal **sp)
{
    x->lowOut = x->notchOut = x->highOut = x->bandOut = 0.0f;
	x->sample_rate = sp[0]->s_sr;
    dsp_add(wah_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

// this input doesn't do anything....
void wah_quality(t_wah *x, t_floatarg g)
{
    if(g > 0.9f) { g = 0.9f;}
    if(g < 0.0f) { g = 0.0f;}
    x->quality = g;
}

static void *wah_new(void)
{
    t_wah *x = (t_wah *)pd_new(wah_class);
    inlet_new(&x->x_obj, &x->x_obj.ob_pd, gensym("float"), gensym("quality"));
    outlet_new(&x->x_obj, gensym("signal"));
    x->quality = 0.0f;
    return (x);
}

// since we allocated some memory, we need a delete function
static void wah_free(t_wah *x)
{
}

    /* this routine, which must have exactly this name (with the "~" replaced
    by "_tilde) is called when the code is first loaded, and tells Pd how
    to build the "class". */
void wah_tilde_setup(void)
{
    wah_class = class_new(gensym("wah~"), (t_newmethod)wah_new, (t_method)wah_free,
    	sizeof(t_wah), 0, A_DEFFLOAT, 0);
	    /* this is magic to declare that the leftmost, "main" inlet
	    takes signals; other signal inlets are done differently... */
		/* also installs delay_time as the leftmost inlet float */
    CLASS_MAINSIGNALIN(wah_class, t_wah, frequency);
    	/* here we tell Pd about the "dsp" method, which is called back
	when DSP is turned on. */
    class_addmethod(wah_class, (t_method)wah_dsp, gensym("dsp"), (t_atomtype)0);
    class_addmethod(wah_class, (t_method)wah_quality, gensym("quality"), A_FLOAT, (t_atomtype)0);
    class_sethelpsymbol(wah_class, gensym("wah~"));
}

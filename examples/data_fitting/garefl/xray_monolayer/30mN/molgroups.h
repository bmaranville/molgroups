//---------------abstract base class---------------------------------------------------------------------
class nSLDObj
{
public:
    nSLDObj();
    virtual ~nSLDObj();
    virtual double fnGetLowerLimit() = 0;
    virtual double fnGetUpperLimit() = 0;
    virtual double fnGetArea(double z) = 0;
    virtual double fnGetnSLD(double z) = 0;
    virtual void   fnSetSigma(double sigma) = 0;
    virtual void   fnSetZ(double dz) {z=dz;};
    virtual double fnGetZ(){return z;};
    virtual double fnGetAbsorb(double z);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    virtual double fnWriteProfile(double aArea[], double anSLD[], double aAbsorb[], int dimension, double stepsize, double dMaxArea);
    virtual void   fnOverlayProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    virtual void   fnOverlayProfile(double aArea[], double anSLD[], double aAbsorb[], int dimension, double stepsize, double dMaxArea);
    virtual void   fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize) = 0;
    virtual void   fnWriteData2File (FILE *fp, const char *cName, int dimension, double stepsize);
    
    bool bWrapping;
    double absorb, z, l, nf, nSL, vol;
};

//------------------------------------------------------------------------------------------------------
class BoxErr : public nSLDObj
{
public:
    BoxErr() {};
    BoxErr(double z, double sigma, double length, double vol, double nSL, double numberfraction);
    virtual ~BoxErr();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void   fnSetSigma(double dsigma) {sigma=dsigma;};
    virtual void   fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    
    double sigma;
};
//------------------------------------------------------------------------------------------------------
class Box2Err : public nSLDObj
{
public:
    Box2Err() {};
    Box2Err(double z, double sigma1, double sigma2, double length, double vol, double nSL, double numberfraction);
    virtual ~Box2Err();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void   fnSetSigma(double sigma);
    virtual void   fnSetSigma(double sigma1, double sigma2);
    virtual void   fnSetZ(double dz);
    virtual void   fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    
    double sigma1, sigma2;
};

//------------------------------------------------------------------------------------------------------

class Gaussian : public nSLDObj
{
public:
    
    Gaussian(double C, double sigma, double vol, double nSL, double numberfraction);
    virtual ~Gaussian();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void   fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    
    double sigma;
};

//------------------------------------------------------------------------------------------------------

class Parabolic: public nSLDObj
{
public:
    Parabolic(double dC, double dH, double dn, double dnSLD, double dnumberfraction);
    virtual ~Parabolic();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void   fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    
    double C, H, n, nSLD;
};
//------------------------------------------------------------------------------------------------------
class StretchGaussian : public nSLDObj
{
public:
    StretchGaussian(double z, double sigma, double length, double vol, double nSL, double numberfraction);
    virtual ~StretchGaussian();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void   fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    
    double sigma;
};


//---------------------------------------------------------------------------------------------------------
class Discrete: public nSLDObj
{
    
public:
	
    Discrete(double dstartposition, double dnormarea, const char *cFileName);
    virtual ~Discrete();
    virtual double fnGetArea(double dz);
    virtual double fnGetnSLD(double dz);
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual void fnSetNormarea(double dnormarea);
    virtual void fnWriteGroup2File(FILE *fp, const char *cName, int dimension, double stepsize);
    
    
    double dStartPosition, dProtExchange, dnSLDBulkSolvent;
    double * area;
    double * nSLProt;
    double * nSLDeut;
    double * zcoord;
    double nf;                  //number of proteins per unit area (typically area per outer leaflet lipid)
    
private:
    int iNumberOfPoints;
    double dZSpacing, normarea;
    
	
};


//---------------------------------------------------------------------------------------------------------
//Freeform group 10 boxes
//---------------------------------------------------------------------------------------------------------


class FreeBox: public nSLDObj
{
    
public:
	
    FreeBox(int n, double dstartposition, double dnSLD, double dnormarea);
    virtual ~FreeBox();
    virtual void fnAdjustParameters();
    virtual double fnGetArea(double dz);
    virtual double fnGetnSLD(double dz);
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual void fnSetStartposition(double dz);
    virtual void fnSetNormarea(double dnormarea);
    virtual void fnSetnSLD(double dnSLD);
    virtual void fnSetSigma(double sigma);
    virtual void fnWriteGroup2File(FILE *fp, const char *cName, int dimension, double stepsize);
    
    Box2Err *box1, *box2, *box3, *box4, *box5, *box6, *box7, *box8, *box9, *box10;
    
    int numberofboxes;
    double vf1, vf2, vf3, vf4, vf5, vf6, vf7, vf8, vf9, vf10;
    double normarea, startposition, nSLD;
    
	
};

//---------------------------------------------------------------------------------------------------------
class Hermite: public nSLDObj
{
    
public:
	
    Hermite(int n, double dstartposition, double dnSLD, double dnormarea);
    virtual ~Hermite();
    virtual double fnGetArea(double dz);
    virtual double fnGetnSLD(double dz);
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual void fnSetNormarea(double dnormarea);
    virtual void fnSetnSLD(double dnSLD);
    virtual void fnSetSigma(double sigma){};
    virtual void fnWriteGroup2File(FILE *fp, const char *cName, int dimension, double stepsize);
    
    
    int numberofcontrolpoints;
    double * vf;
    double * dp;
    double normarea, nSLD;
    
	
};

//------------------------------------------------------------------------------------------------------
//PC headgroup
class PC: public nSLDObj
{
protected:
    double z;
public:
    PC();
    virtual ~PC();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual double fnGetZ() {return z;};
    virtual void fnSetSigma(double sigma);
    virtual void fnSetZ(double dz);
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    
    Box2Err *cg;
    Box2Err *phosphate;
    Box2Err *choline;
};
//------------------------------------------------------------------------------------------------------
class PCm: public PC
{
public:
    PCm();
    virtual ~PCm();
    virtual void fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
};
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//AminoAcid
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//general implementation
//-----------------------------------------------------------------------------------------------------------
class AminoAcid: public BoxErr
{
public:
    AminoAcid();
    virtual ~AminoAcid() {};
    virtual double fnGetnSLD(double z);
    
    int nH, nExch, Deuterated;
    double ExchangeRatio;
};
//-----------------------------------------------------------------------------------------------------------
//Specific implementations
//-----------------------------------------------------------------------------------------------------------
class AA_Lys: public AminoAcid
{
public:
    AA_Lys();
    virtual ~AA_Lys() {};
};
class AA_Arg: public AminoAcid
{
public:
    AA_Arg();
    virtual ~AA_Arg() {};
};
class AA_His: public AminoAcid
{
public:
    AA_His();
    virtual ~AA_His() {};
};
class AA_Asn: public AminoAcid
{
public:
    AA_Asn();
    virtual ~AA_Asn() {};
};
class AA_Asp: public AminoAcid
{
public:
    AA_Asp();
    virtual ~AA_Asp() {};
};
class AA_Cys: public AminoAcid
{
public:
    AA_Cys();
    virtual ~AA_Cys() {};
};
class AA_Thr: public AminoAcid
{
public:
    AA_Thr();
    virtual ~AA_Thr() {};
};
class AA_Ser: public AminoAcid
{
public:
    AA_Ser();
    virtual ~AA_Ser() {};
};
class AA_Gln: public AminoAcid
{
public:
    AA_Gln();
    virtual ~AA_Gln() {};
};
class AA_Glu: public AminoAcid
{
public:
    AA_Glu();
    virtual ~AA_Glu() {};
};
class AA_Pro: public AminoAcid
{
public:
    AA_Pro();
    virtual ~AA_Pro() {};
};
class AA_Gly: public AminoAcid
{
public:
    AA_Gly();
    virtual ~AA_Gly() {};
};
class AA_Ala: public AminoAcid
{
public:
    AA_Ala();
    virtual ~AA_Ala() {};
};
class AA_Val: public AminoAcid
{
public:
    AA_Val();
    virtual ~AA_Val() {};
};
class AA_Ile: public AminoAcid
{
public:
    AA_Ile();
    virtual ~AA_Ile() {};
};
class AA_Leu: public AminoAcid
{
public:
    AA_Leu();
    virtual ~AA_Leu() {};
};
class AA_Met: public AminoAcid
{
public:
    AA_Met();
    virtual ~AA_Met() {};
};
class AA_Tyr: public AminoAcid
{
public:
    AA_Tyr();
    virtual ~AA_Tyr() {};
};
class AA_Phe: public AminoAcid
{
public:
    AA_Phe();
    virtual ~AA_Phe() {};
};
class AA_Trp: public AminoAcid
{
public:
    AA_Trp();
    virtual ~AA_Trp() {};
};

//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//Lipid bilayers
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//General Implementations
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//monlayer, single-lipid bilayer
//------------------------------------------------------------------------------------------------------
class Monolayer: public nSLDObj
{
protected:
    double normarea;
public:
    Monolayer();
    virtual ~Monolayer();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void   fnSetSigma(double sigma);
    virtual void   fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    virtual double fnWriteProfile(double aArea[], double anSLD[], double aAbsorb[], int dimension, double stepsize, double dMaxArea);
    
    Box2Err   *substrate;
    nSLDObj   *headgroup;                                               
    Box2Err   *lipid;
    Box2Err   *methyl;
    
    //primary fit parameters
    double global_rough, sigma, l_lipid, vf_bilayer, rho_substrate, absorb_substrate;
    double hc_substitution;
    
    //other parameters
    double volacyllipid, nslacyllipid, volmethyllipid, nslmethyllipid, absorbacyllipid, absorbmethyllipid;
};


//------------------------------------------------------------------------------------------------------
//solid supported lipid bilayer, single-lipid bilayer
//------------------------------------------------------------------------------------------------------
class ssBLM: public nSLDObj
{
protected:
    double normarea;
public:
    ssBLM();
    virtual ~ssBLM();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    
    Box2Err   *substrate;
    PCm       *headgroup1;                                                 //mirrored PC head group
    Box2Err   *lipid1;
    Box2Err   *methyl1;
    Box2Err   *methyl2;
    Box2Err   *lipid2;
    PC        *headgroup2;                                                //PC head group
    
    //primary fit parameters
    double global_rough, sigma, l_lipid1, l_lipid2, vf_bilayer, rho_substrate, l_submembrane;
    double hc_substitution_1, hc_substitution_2;
    
    //other parameters
    double volacyllipid, nslacyllipid, volmethyllipid, nslmethyllipid;
};


//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//tethered lipid bilayer, single-lipid bilayer
class tBLM: public nSLDObj
{
protected:
    double normarea;
public:
    tBLM();
    virtual ~tBLM();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    
    Box2Err   *substrate;
    Box2Err   *bME;
    Box2Err   *tether;
    Box2Err   *tetherg; 
    PCm       *headgroup1;                                                 //mirrored PC head group
    Box2Err   *lipid1;
    Box2Err   *methyl1;
    Box2Err   *methyl2;
    Box2Err   *lipid2;
    PC        *headgroup2;                                                //PC head group
    
    //primary fit parameters
    double global_rough, sigma, l_lipid1, l_lipid2, vf_bilayer, l_tether, nf_tether, mult_tether, rho_substrate;
    double hc_substitution_1, hc_substitution_2;
    
    //other parameters
    double volacyllipid, nslacyllipid, volmethyllipid, nslmethyllipid, volmethyltether;
    double nslmethyltether, volacyltether, nslacyltether;
    
};
//------------------------------------------------------------------------------------------------------
//tethered lipid bilayer, binary lipid bilayer
class tBLM_binary: public tBLM
{
protected:
    double normarea;
public:
    tBLM_binary();
    virtual ~tBLM_binary();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    
    Box2Err	  *headgroup1_2;	  
    Box2Err   *headgroup2_2;
    
    //primary fit parameters
    double nf_lipid_2, nf_lipid1_2, nf_lipid2_2;
    
    int iSeparateLeafletComposition;
    
    //other parameters
    double volacyllipid_2, nslacyllipid_2, volmethyllipid_2, nslmethyllipid_2;
};
//------------------------------------------------------------------------------------------------------
//tethered lipid bilayer, ternary lipid bilayer
class tBLM_ternary: public tBLM
{
protected:
    double normarea;
public:
    tBLM_ternary();
    virtual ~tBLM_ternary();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    
    Box2Err	  *headgroup1_2;	  
    Box2Err   *headgroup2_2;
    Box2Err	  *headgroup1_3;	  
    Box2Err   *headgroup2_3;
    
    //primary fit parameters
    double nf_lipid_2;
    double nf_lipid_3;
    
    //other parameters
    double volacyllipid_2, nslacyllipid_2, volmethyllipid_2, nslmethyllipid_2;
    double volacyllipid_3, nslacyllipid_3, volmethyllipid_3, nslmethyllipid_3;
};
//------------------------------------------------------------------------------------------------------
//tethered lipid bilayer, ternary lipid bilayer
class tBLM_ternary_chol: public tBLM
{
protected:
    double normarea;
public:
    tBLM_ternary_chol();
    virtual ~tBLM_ternary_chol();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    
    Box2Err	  *headgroup1_2;	  
    Box2Err   *headgroup2_2;
    
    //primary fit parameters
    double nf_lipid_2;
    double nf_chol;
    
    //other parameters
    double volacyllipid_2, nslacyllipid_2, volmethyllipid_2, nslmethyllipid_2;
    double volchol, nslchol;
};

//------------------------------------------------------------------------------------------------------
//tethered lipid bilayer, quaternary lipid bilayer
class tBLM_quaternary_chol: public tBLM
{
protected:
    double normarea;
public:
    tBLM_quaternary_chol();
    virtual ~tBLM_quaternary_chol();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    
    Box2Err	  *headgroup1_2;	  
    Box2Err   *headgroup2_2;
    Box2Err	  *headgroup1_3;	  
    Box2Err   *headgroup2_3;
    
    //primary fit parameters
    double nf_lipid_2;
    double nf_lipid_3;
    double nf_chol;
    
    //other parameters
    double volacyllipid_2, nslacyllipid_2, volmethyllipid_2, nslmethyllipid_2;
    double volacyllipid_3, nslacyllipid_3, volmethyllipid_3, nslmethyllipid_3;
    double volchol, nslchol;
};

//------------------------------------------------------------------------------------------------------
//tethered lipid bilayer, quaternary lipid bilayer
//there will be a domains implemented in a way that 
//there is a separate set of nf_..._domain parameters for 
//the second domain, and that there is a frac_domain parameter
//determining the surface fraction of the second domain

class tBLM_quaternary_chol_domain: public tBLM
{
protected:
    double normarea, normarea_domain;
public:
    tBLM_quaternary_chol_domain();
    virtual ~tBLM_quaternary_chol_domain();
    virtual void   fnAdjustParameters();
    virtual double fnGetLowerLimit();
    virtual double fnGetUpperLimit();
    virtual double fnGetArea(double z);
    virtual double fnGetnSLD(double z);
    virtual void fnWriteGroup2File (FILE *fp, const char *cName, int dimension, double stepsize);
    virtual double fnWriteProfile(double aArea[], double anSLD[], int dimension, double stepsize, double dMaxArea);
    
    Box2Err	  *headgroup1_2;	  
    Box2Err   *headgroup2_2;
    Box2Err	  *headgroup1_3;	  
    Box2Err   *headgroup2_3;

    PCm       *headgroup1_domain;
    Box2Err   *lipid1_domain;
    Box2Err   *methyl1_domain;
    Box2Err   *methyl2_domain;
    Box2Err   *lipid2_domain;
    PC        *headgroup2_domain;
    Box2Err	  *headgroup1_2_domain;	  
    Box2Err   *headgroup2_2_domain;
    Box2Err	  *headgroup1_3_domain;	  
    Box2Err   *headgroup2_3_domain;
    Box2Err   *tetherg_domain;
    Box2Err   *tether_domain;

    
    //primary fit parameters
    double nf_lipid_2;
    double nf_lipid_3;
    double nf_chol;
    
    double nf_lipid_2_domain;
    double nf_lipid_3_domain;
    double nf_chol_domain;
    
    //frac_domain is a molar fraction
    double frac_domain, l_lipid1_domain, l_lipid2_domain, l_tether_domain;
    
    //other parameters
    double volacyllipid_2, nslacyllipid_2, volmethyllipid_2, nslmethyllipid_2;
    double volacyllipid_3, nslacyllipid_3, volmethyllipid_3, nslmethyllipid_3;
    double volchol, nslchol;
};

//------------------------------------------------------------------------------------------------------
// Bilayer Library -- Monolayer
//-----------------------------------------------------------------------------------------------------------

class Monolayer_DOPS: public Monolayer
{
public:
    Monolayer_DOPS();
    virtual ~Monolayer_DOPS();
};
class Monolayer_DPPS: public Monolayer
{
public:
    Monolayer_DPPS();
    virtual ~Monolayer_DPPS();
};

//-----------------------------------------------------------------------------------------------------------
// Bilayer Library -- ssBLM
//-----------------------------------------------------------------------------------------------------------

class ssBLM_POPC: public ssBLM
{
public:
    ssBLM_POPC();
    virtual ~ssBLM_POPC() {};
};


//------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
// tBLM Library -- tBLM
//-----------------------------------------------------------------------------------------------------------
class tBLM_HC18_DOPC: public tBLM
{
public:
    tBLM_HC18_DOPC();
    virtual ~tBLM_HC18_DOPC() {};
};

class tBLM_HC18_d54DMPC: public tBLM
{
public:
    tBLM_HC18_d54DMPC();
    virtual ~tBLM_HC18_d54DMPC() {};
};

class tBLM_WC14_DOPC: public tBLM
{
public:
    tBLM_WC14_DOPC();
    virtual ~tBLM_WC14_DOPC() {};
};
//-----------------------------------------------------------------------------------------------------------
// tBLM Library -- tBLM_binary
//-----------------------------------------------------------------------------------------------------------
class tBLM_HC18_POPC_POPA: public tBLM_binary
{
public:
    tBLM_HC18_POPC_POPA();
    virtual ~tBLM_HC18_POPC_POPA() {};
};

class tBLM_HC18_POPC_POPG: public tBLM_binary
{
public:
    tBLM_HC18_POPC_POPG();
    virtual ~tBLM_HC18_POPC_POPG() {};
};

class tBLM_HC18_DOPC_DOPS: public tBLM_binary
{
public:
    tBLM_HC18_DOPC_DOPS();
    virtual ~tBLM_HC18_DOPC_DOPS() {};
};

class tBLM_HC18_DOPC_d54DMPC: public tBLM_binary
{
public:
    tBLM_HC18_DOPC_d54DMPC();
    virtual ~tBLM_HC18_DOPC_d54DMPC() {};
};

class tBLM_HC18_DMPC_d54DMPC: public tBLM_binary
{
public:
    tBLM_HC18_DMPC_d54DMPC();
    virtual ~tBLM_HC18_DMPC_d54DMPC() {};
};

class tBLM_WC14_DOPC_DOPS: public tBLM_binary
{
public:
    tBLM_WC14_DOPC_DOPS();
    virtual ~tBLM_WC14_DOPC_DOPS() {};
};

class tBLM_WC14_DOPC_PIP: public tBLM_binary
{
public:
    tBLM_WC14_DOPC_PIP();
    virtual ~tBLM_WC14_DOPC_PIP() {};
};

class tBLM_HC18_DOPC_PIP: public tBLM_binary
{
public:
    tBLM_HC18_DOPC_PIP();
    virtual ~tBLM_HC18_DOPC_PIP() {};
};
//-----------------------------------------------------------------------------------------------------------
// tBLM Library -- tBLM_ternary
//-----------------------------------------------------------------------------------------------------------
class tBLM_HC18_DOPC_DOPS_PIP: public tBLM_ternary
{
public:
    tBLM_HC18_DOPC_DOPS_PIP();
    virtual ~tBLM_HC18_DOPC_DOPS_PIP() {};
};

class tBLM_WC14_DOPC_DOPS_CHOL: public tBLM_ternary_chol
{
public:
    tBLM_WC14_DOPC_DOPS_CHOL();
    virtual ~tBLM_WC14_DOPC_DOPS_CHOL() {};
};

class tBLM_HC18_DOPC_DOPS_CHOL: public tBLM_ternary_chol
{
public:
    tBLM_HC18_DOPC_DOPS_CHOL();
    virtual ~tBLM_HC18_DOPC_DOPS_CHOL() {};
};

//-----------------------------------------------------------------------------------------------------------
// tBLM Library -- tBLM_quaternary
//-----------------------------------------------------------------------------------------------------------

class tBLM_HC18_DOPC_DOPS_PIP_CHOL: public tBLM_quaternary_chol
{
public:
    tBLM_HC18_DOPC_DOPS_PIP_CHOL();
    virtual ~tBLM_HC18_DOPC_DOPS_PIP_CHOL() {};
};

//-----------------------------------------------------------------------------------------------------------
// tBLM Library -- tBLM_quaternary_domain
//-----------------------------------------------------------------------------------------------------------

class tBLM_HC18_DOPC_DOPS_PIP_CHOL_domain: public tBLM_quaternary_chol_domain
{
public:
    tBLM_HC18_DOPC_DOPS_PIP_CHOL_domain();
    virtual ~tBLM_HC18_DOPC_DOPS_PIP_CHOL_domain() {};
};


//-----------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------
// Standalone
//------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------

void fnWriteConstant(FILE *fp, const char *cName, double area, double nSLD, int dimension, double stepsize);
void fnOverlayCanvasOnCanvas(double aArea[], double anSL[], double aArea2[], double anSL2[], int dimension, double dMaxArea);
void fnOverlayCanvasOnCanvas(double aArea[], double anSL[], double aAbsorb[], double aArea2[], double anSL2[], double aAbsorb2[], int dimension, double dMaxArea);
double fnClearCanvas(double aArea[], double anSL[], int dimension);
double fnClearCanvas(double aArea[], double anSL[], double aAbsorb[], int dimension);
void fnWriteCanvas2Model(double aArea[], double anSL[], fitinfo fit[], int gaussstart, int dimension, double stepsize, double dMaxArea, double normarea, int modelstart, int modelend);
void fnWriteCanvas2Model(double aArea[], double anSL[], double aAbsorb[], fitinfo fit[], int gaussstart, int dimension, double stepsize, double dMaxArea, double normarea, int modelstart, int modelend);
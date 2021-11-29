## === Import section ===
import sys
# append path to your molgroups, or just link molgroups to your same directory
sys.path.append("../../../../src/molgroups")
import numpy as np
import molgroups as mol
import components as cmp
from refl1d.names import load4, Parameter, SLD, Slab, Stack, Experiment, FitProblem
from refl1d.flayer import FunctionalProfile

## === Film structure definition section ===

### Bilayer profile definition function

def bilayer(z, sigma, bulknsld, global_rough, rho_substrate, l_submembrane, l_lipid1, l_lipid2, vf_bilayer):
    """ Fairly generic bilayer. This assumes a stack of materials already existing because siox.l is set to zero """
    
    # Set unused parameters
    l_siox = 0.0 # could make a parameter in the future
    rho_siox = 0.0

    # Scale all SLDs from Refl1D units (1e-6 Ang^-2) to molgroups units (Ang^-2)
    bulknsld *= 1e-6
    rho_substrate *= 1e-6

    blm.fnSet(sigma, bulknsld, global_rough, rho_substrate, rho_siox, l_siox, l_submembrane, l_lipid1, l_lipid2, vf_bilayer)
    
    # Calculate scattering properties of volume occupied by bilayer
    normarea, area, nsl = blm.fnWriteProfile(z)

    # Fill in the remaining volume with buffer of appropriate nSLD
    nsld = nsl / (normarea * np.gradient(z)) + (1.0 - area / normarea) * bulknsld

    # Return nSLD profile in Refl1D units
    return nsld*1e6

### Define bilayer parameters
vf_bilayer = Parameter(name='volume_fraction_bilayer', value=0.9).range( 0.0, 1.0)
l_lipid1 = Parameter(name='inner_acyl_chain_thickness', value=10.0).range( 8.0, 16.0)
l_lipid2 = Parameter(name='outer_acyl_chain_thickness', value=10.0).range( 8.0, 16.0)
sigma = Parameter(name='bilayer_roughness', value=5).range( 2.0, 9.0)
global_rough = Parameter(name ='tiox_roughness', value=5).range(2, 9)
l_tiox = Parameter(name='total_tiox_thickness', value=120).range(50, 150)
l_submembrane = Parameter(name='submembrane_thickness', value=10).range( 0.0, 50.0)

### Define bilayer object
DOPC = cmp.Lipid(name='DOPC', headgroup=mol.PC, tails=[cmp.oleoyl, cmp.oleoyl], methyls=cmp.methyl)
blm = mol.ssBLM(lipids=[DOPC], lipid_nf=[1.0])

### Define molgroups space.
dimension=300       # Number of steps
stepsize=0.5        # Length of steps

## === Stack ===
##
## First, we create a 'material' for each bulk layer, which has an real and imaginary
## scattering length density, stored in a Refl1d object called 'SLD'
d2o = SLD(name='d2o', rho=6.3000, irho=0.0000)
h2o = SLD(name='h2o', rho=-0.56, irho=0.0000)
tiox = SLD(name='tiox', rho=2.1630, irho=0.0000)
siox = SLD(name='siox', rho=4.1000, irho=0.0000)
silicon = SLD(name='silicon', rho=2.0690, irho=0.0000)

## Then bulk layers are created, each with its own 'material'.  If you want to force
## two layers to always match SLD you can use the same material in multiple layers.
## The roughnesses of each layer are set to zero to begin with:

layer_d2o = Slab(material=d2o, thickness=0.0000, interface=5.0000)
layer_h2o = Slab(material=h2o, thickness=0.0000, interface=5.0000)
layer_tiox = Slab(material=tiox, thickness=l_tiox - (blm.substrate.z + 0.5 * blm.substrate.l), interface=0.0)
layer_siox = Slab(material=siox, thickness=7.5804, interface=10.000)
layer_silicon = Slab(material=silicon, thickness=0.0000, interface=0.0000)

## Use the bilayer definition function to generate the bilayer SLD profile, passing in the relevant parameters.
## Note that substrate and bulk SLDs are linked to their respective materials.
mollayer = FunctionalProfile(dimension*stepsize, 0, profile=bilayer, sigma=sigma,
                                bulknsld=d2o.rho, global_rough=global_rough, rho_substrate=tiox.rho,
                                l_submembrane=l_submembrane, l_lipid1=l_lipid1, l_lipid2=l_lipid2,
                                vf_bilayer=vf_bilayer)
mollayerh = FunctionalProfile(dimension*stepsize, 0, profile=bilayer, sigma=sigma,
                                bulknsld=h2o.rho, global_rough=global_rough, rho_substrate=tiox.rho,
                                l_submembrane=l_submembrane, l_lipid1=l_lipid1, l_lipid2=l_lipid2,
                                vf_bilayer=vf_bilayer)

## Stack the layers into individual samples, using common layer objects for layers that are unchanged between samples
## As a convention, always build the sample from the substrate up. If the neutron beam is incident from the substrate side,
## set back_reflectivity = True in the probe definition later.

sample = layer_silicon | layer_siox | layer_tiox | mollayer | layer_d2o
sampleh = layer_silicon | layer_siox | layer_tiox | mollayerh | layer_h2o

## Set sample parameter ranges and constraints between layer properties

# nSLD parameters
d2o.rho = rho_d2o = Parameter(name='rho_solv_0', value=6.34).range( 5.3, 6.5)
h2o.rho = rho_h2o = Parameter(name='rho_solv_1', value=-0.56).range( -0.6, 0.6)
tiox.rho = rho_tiox = Parameter(name='rho_tiox', value=2).range( -2.15829, -0.15829000000000004)
siox.rho = rho_siox = Parameter(name='rho_siox', value=3.5).range( 3.1, 5.1)

# layer thickness parameters
layer_tiox.thickness = tiox_thickness = Parameter(name='tiox_thickness', value=100.).range( 66.379, 266.38)
layer_siox.thickness = siox_thickness = Parameter(name='siox_thickness', value=30.).range( 5.0, 40.0)

# layer roughness parameters
###################################################################
## the 'interface' associated with layer0 is the boundary between #
## layer0 and layer1, and similarly for layer(N) and layer(N+1)   #
###################################################################
layer_siox.interface = siox_interface = Parameter(name='siox_interface', value=5.0).range( 2.0, 9.0)

# Si and SiOx roughnesses are the same
layer_silicon.interface = layer_siox.interface

## === Data files ===
probe = load4('sim1.dat', back_reflectivity=True)
probeh = load4('sim0.dat', back_reflectivity=True)

# Set instrumental (probe) parameters
probe.background = background_0 = Parameter(name='background_0', value=0.).range( -1e-07, 1e-05)
probeh.background = background_1 = Parameter(name='background_1', value=0.).range( -1e-07, 1e-05)
probe.intensity = intensity = Parameter(name='intensity', value=1.0).range( 0.9, 1.05)
probeh.intensity = probe.intensity
probe.theta_offset = theta_offset = Parameter(name='theta_offset', value=0.).range( -0.015, 0.005)
probeh.theta_offset = probe.theta_offset
probe.sample_broadening = sample_broadening = Parameter(name='sample_broadening', value=0).range( -0.005, 0.02)
probeh.sample_broadening = probe.sample_broadening

# Define critical edge oversampling for samples that require it
probe.critical_edge(substrate=silicon, surface=d2o)

## === Problem definition ===
## a model object consists of a sample and a probe.

## step = True corresponds to a calculation of the reflectivity from an actual profile
## with microslabbed interfaces.  When step = False, the Nevot-Croce
## approximation is used to account for roughness.  This approximation speeds up
## the calculation tremendously, and is reasonably accuarate as long as the
## roughness is much less than the layer thickness
step = False

model = Experiment(sample=sample, probe=probe, dz=stepsize, step_interfaces = step)
modelh = Experiment(sample=sampleh, probe=probeh, dz=stepsize, step_interfaces = step)

problem = FitProblem([model, modelh])

## === Export objects for post analysis ===
problem.name = "DOPC bilayer on TiOx substrate"
problem.bilayers = [blm]
problem.dimension = dimension
problem.stepsize = stepsize

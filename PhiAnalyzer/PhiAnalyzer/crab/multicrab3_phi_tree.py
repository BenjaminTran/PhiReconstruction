import phianalyzerheader as phi
from phianalyzerheader import os

version = 3
collID = 'pPb'
#collID = 'Pbp'
#collID = 'pPbMB'
#collID = 'PbpMB'
#collID = 'PbPb'


phi.config.General.workArea = 'crab_dir/Phi_Tree_2016pPb'
phi.config.JobType.priority = 15

phi.config.JobType.psetName = os.path.expandvars('$CMSSW_BASE/src/PhiAnalyzer/PhiAnalyzer/python/phitree_cfg.py')

if __name__ == '__main__':

    from CRABAPI.RawCommand import crabCommand
    from CRABClient.ClientExceptions import ClientException
    from httplib import HTTPException

    def submit(config):
        try:
            crabCommand('submit', config = config)
        except HTTPException as hte:
            print "Failed submitting task: %s" % (hte.headers)
        except ClientException as cle:
            print "Failed submitting task: %s" % (cle)


    if collID == 'pPb' or collID == 'Pbp':
        for num in range(0,1):
            try:
                with open( 'PhiVarStore.dat', 'r' ) as fle:
                    counter = int( fle.readline() )
            except FileNotFoundError:
                counter = 0

            phi.config.Data.outputDatasetTag = 'PhiTree_JL' + str(counter)

            if collID == 'pPb':
                DataSet = ['/PAHighMultiplicity1/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity2/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity3/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity4/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity5/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity6/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER']
                print 'Input Dataset is %r ' % (DataSet[num])
                phi.config.Data.inputDataset = DataSet[num]
                phi.config.General.requestName = 'PhiTree_2016pPbPD' + str(num+1) + '_v' + str(version) + '_JL' + str(counter)
            else:
                DataSet = ['/PAHighMultiplicity1/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity2/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity3/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity4/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity5/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAHighMultiplicity6/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER']
                print 'Input Dataset is %r ' % (DataSet[num])
                phi.config.Data.inputDataset = DataSet[num]
                phi.config.General.requestName = 'PhiTree_2016PbpPD' + str(num+1) + '_v' + str(version) + '_JL' + str(counter)
            with open( 'PhiVarStore.dat', 'w' ) as fle:
                counter = counter + 1
                fle.write( str(counter) )
            submit(phi.config)
    if collID == 'pPbMB' or collID =='PbpMB':
        if collID == 'pPbMB':
            for num in range(1,8):
                try:
                    with open( 'PhiVarStore.dat', 'r' ) as fle:
                        counter = int( fle.readline() )
                except FileNotFoundError:
                    counter = 0

                DataSet = ['/PAMinimumBias1/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias2/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias3/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias4/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias5/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias6/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias7/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias8/davidlw-RecoSkim2016_pPb_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER']
                print 'Input Dataset is %r ' % (DataSet[num])
                phi.config.Data.inputDataset = DataSet[num]
                phi.config.General.workArea = 'crab_dir/HLT185_250FlowCombinedv2ppbMB2016PD' + str(num+1) + 'Rap'
                phi.config.General.requestName = 'HLT185_250Flow2016CorrelationpPbMBCombinedPD' + str(num+1) + 'JL' + str(counter)
                with open( 'PhiVarStore.dat', 'w' ) as fle:
                    counter = counter + 1
                    fle.write( str(counter) )
                submit(phi.config)
        else:
            for num in range(7,13):
                try:
                    with open( 'PhiVarStore.dat', 'r' ) as fle:
                        counter = int( fle.readline() )
                except FileNotFoundError:
                    counter = 0

                DataSet = ['/PAMinimumBias1/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias2/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias3/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias4/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias5/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias6/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias7/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias8/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias9/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias10/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias11/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias12/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias13/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias14/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias15/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias16/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias17/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias18/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias19/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER',
                        '/PAMinimumBias20/davidlw-RecoSkim2016_Pbp_V0Cascade_v1-97be9aa52ea60cba5455e64649c12464/USER']
                print 'Input Dataset is %r ' % (DataSet[num])
                phi.config.Data.inputDataset = DataSet[num]
                phi.config.General.workArea = 'crab_dir/HLT185_250FlowCombinedv2PbpMB2016PD' + str(num+1) + 'Rap'
                phi.config.General.requestName = 'HLT185_250Flow2016CorrelationPbpMBCombinedPD' + str(num+1) + 'JL' + str(counter)
                with open( 'PhiVarStore.dat', 'w' ) as fle:
                    counter = counter + 1
                    fle.write( str(counter) )
                submit(phi.config)
    if collID == 'PbPb':
            for num in range(0,3):
                try:
                    with open( 'PhiVarStore.dat', 'r' ) as fle:
                        counter = int( fle.readline() )
                except FileNotFoundError:
                    counter = 0

                DataSet = ['/HIMinimumBias5/davidlw-RecoSkim2015_pprereco_V0Cascade_Golden_v2-a2a36526d6b050b4e6f00846a47a9f83/USER',
                        '/HIMinimumBias6/davidlw-RecoSkim2015_pprereco_V0Cascade_Golden_v2-a2a36526d6b050b4e6f00846a47a9f83/USER',
                        '/HIMinimumBias7/davidlw-RecoSkim2015_pprereco_V0Cascade_Golden_v2-a2a36526d6b050b4e6f00846a47a9f83/USER']
                print 'Input Dataset is %r ' % (DataSet[num])
                phi.config.Data.inputDataset = DataSet[num]
                phi.config.General.workArea = 'crab_dir/HLT185_250FlowCombinedv2PbPb2016PD' + str(num+5) + 'Rap'
                phi.config.General.requestName = 'Cent30_50_Flow2016CorrelationPbPbCombinedPD' + str(num+5) + 'JL' + str(counter)
                with open( 'PhiVarStore.dat', 'w' ) as fle:
                    counter = counter + 1
                    fle.write( str(counter) )
                submit(phi.config)



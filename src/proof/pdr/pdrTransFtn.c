/**CFile****************************************************************

  FileName    [pdrTransFtn.c]

  SystemName  [ABC: Logic synthesis and verification system.]

  PackageName []

  Synopsis    [Transform a transition relation into a sequential aig.]

  Author      [Alan Mishchenko]

  Affiliation [UC Berkeley]

  Date        [Ver. 1.0. Started - June 20, 2005.]

  Revision    [$Id: .c,v 1.00 2005/06/20 00:00:00 alanmi Exp $]

***********************************************************************/

#include "base/abc/abc.h"
#include "aig/aig/aig.h"
#include "pdrInt.h"

ABC_NAMESPACE_IMPL_START

////////////////////////////////////////////////////////////////////////
///                        DECLARATIONS                              ///
////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
///                     FUNCTION DEFINITIONS                         ///
////////////////////////////////////////////////////////////////////////

/**Function*************************************************************

  Synopsis    [Constructs the sequential AIG (transition function) from the transition relation.]

  Description []

  SideEffects []

  SeeAlso     []

***********************************************************************/
Abc_Ntk_t *Abc_NtkTransFtn(Abc_Ntk_t *pNtk)
{
    Abc_Ntk_t *pNtkNew;
    Abc_Obj_t *pObj, *pZIn, *pZOut, *pZ, *pT, * pO;
    int i, nLatches;

    assert(Abc_NtkIsStrash(pNtk));
    assert(!Abc_NtkLatchNum(pNtk));
    assert(Abc_NtkPiNum(pNtk) % 2 == 0);
    nLatches = Abc_NtkPiNum(pNtk) >> 1;
    // start the network
    pNtkNew = Abc_NtkAlloc(ABC_NTK_STRASH, ABC_FUNC_AIG, 1);
    // duplicate the name and the spec
    pNtkNew->pName = Extra_UtilStrsav(pNtk->pName);
    pNtkNew->pSpec = Extra_UtilStrsav(pNtk->pSpec);
    Abc_NtkCleanCopy(pNtk);

    // Create the PO
    pObj = Abc_NtkCreatePo(pNtkNew);
    Abc_ObjAssignName( pObj, "PO", NULL );
    
    // Create the PIs
    for (i = 0; i <nLatches; ++i)
    {
        pObj = Abc_NtkPi(pNtk, i + nLatches);
        pObj->pCopy = Abc_NtkCreatePi(pNtkNew);
        Abc_ObjAssignName( pObj->pCopy, Abc_ObjName(pObj), NULL );
    }
    // Create the latch Z
    pZOut = Abc_NtkAddLatch(pNtkNew, NULL, ABC_INIT_ZERO);
    pZ = Abc_ObjFanin0(pZOut);
    pZIn = Abc_ObjFanin0(pZ);
    // Create the Latches
    for (i = 0; i <nLatches; ++i)
    {
        pObj = Abc_NtkPi(pNtk, i);
        pObj->pCopy = Abc_NtkAddLatch(pNtkNew, NULL, ABC_INIT_ZERO);
    }
    // restrash the nodes (assuming a topological order of the old network)
    Abc_AigConst1(pNtk)->pCopy = Abc_AigConst1(pNtkNew);
    Abc_NtkForEachNode(pNtk, pObj, i)
        pObj->pCopy = Abc_AigAnd((Abc_Aig_t *)pNtkNew->pManFunc, Abc_ObjChild0Copy(pObj), Abc_ObjChild1Copy(pObj));
    // create the function of the PO and latches
    assert(Abc_NtkBoxNum(pNtkNew) == Abc_NtkLatchNum(pNtkNew));
    pT = Abc_ObjChild0Copy(Abc_NtkPo(pNtk, 0));
    pO = Abc_ObjChild0Copy(Abc_NtkPo(pNtk, 1));
    // Feed the PO
    Abc_ObjAddFanin(Abc_NtkPo(pNtkNew, 0), Abc_AigAnd((Abc_Aig_t *)pNtkNew->pManFunc, pO, Abc_ObjNot(pZOut)));

    // Feed the latches
    Abc_NtkForEachLatchInput(pNtkNew, pObj, i)
    {
        if (i == 0)
        {
            assert(pObj == pZIn);
            Abc_ObjAddFanin(pObj, Abc_AigOr((Abc_Aig_t *)pNtkNew->pManFunc, Abc_ObjNot(pT), pZOut));
        }
        else
        {
            Abc_ObjAddFanin(pObj, Abc_NtkPi(pNtkNew, i - 1));
        }
    }
    // check consistency of the network
    if (!Abc_NtkCheck(pNtkNew))
    {
        printf("Abc_NtkTransFtn: The network check has failed.\n");
        Abc_NtkDelete(pNtkNew);
        return NULL;
    }

    // check consistency of the network
    if (!Abc_NtkCheck(pNtkNew))
    {
        printf("Abc_NtkTransRel: The network check has failed.\n");
        Abc_NtkDelete(pNtkNew);
        return NULL;
    }
    return pNtkNew;
}

/**Function*************************************************************

  Synopsis    []

  Description []

  SideEffects []

  SeeAlso     []

***********************************************************************/

////////////////////////////////////////////////////////////////////////
///                       END OF FILE                                ///
////////////////////////////////////////////////////////////////////////

ABC_NAMESPACE_IMPL_END

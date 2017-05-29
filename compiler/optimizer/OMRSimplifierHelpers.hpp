/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2000, 2016
 *
 *  This program and the accompanying materials are made available
 *  under the terms of the Eclipse Public License v1.0 and
 *  Apache License v2.0 which accompanies this distribution.
 *
 *      The Eclipse Public License is available at
 *      http://www.eclipse.org/legal/epl-v10.html
 *
 *      The Apache License v2.0 is available at
 *      http://www.opensource.org/licenses/apache2.0.php
 *
 * Contributors:
 *    Multiple authors (IBM Corp.) - initial implementation and documentation
 *******************************************************************************/

#ifndef OMR_SIMPLIFIERHELPERS_INCL
#define OMR_SIMPLIFIERHELPERS_INCL

#include "il/DataTypes.hpp"                    // for DataTypes
#include "il/ILOps.hpp"                        // for TR::ILOpCode, ILOpCode

namespace TR { class Block; }
namespace TR { class Compilation; }
namespace TR { class Node; }
namespace TR { class Optimization; }
namespace TR { class TreeTop; }
namespace TR { class Simplifier; }
class TR_RegionStructure;
class TR_FrontEnd;

enum {XXCMP_EQ = 0, XXCMP_LT = 1, XXCMP_GT = 2};

#define XXCMP_TABLE {0, -1, 1}

#define XXCMP_SIMPLIFIER(node, block, s, Type)                          \
{                                                                       \
 simplifyChildren(node, block, s);                                      \
 TR::Node *firstChild  = node->getFirstChild();                          \
 TR::Node *secondChild = node->getSecondChild();                         \
 int8_t table[3] = XXCMP_TABLE;                                         \
 if (firstChild == secondChild)                                         \
    foldByteConstant(node, table[XXCMP_EQ], s, true /* anchorChildren */); \
 else if (firstChild->getOpCode().isLoadConst() &&                      \
          secondChild->getOpCode().isLoadConst())                       \
    {                                                                   \
    if (firstChild->get##Type() > secondChild->get##Type())             \
       foldByteConstant(node, table[XXCMP_GT], s, false /* !anchorChildren*/); \
    else if (firstChild->get##Type() < secondChild->get##Type())        \
       foldByteConstant(node, table[XXCMP_LT], s, false /* !anchorChildren*/); \
    else if (firstChild->get##Type() == secondChild->get##Type())       \
       foldByteConstant(node, table[XXCMP_EQ], s, false /* !anchorChildren*/); \
    }                                                                   \
}

//**************************************
// Binary identity operation
//
// If the second child is a constant that represents an identity operation,
// replace this node with the first child.
//
#define BINARY_IDENTITY_OP(Type,NullValue)               \
   if (secondChild->getOpCode().isLoadConst() && secondChild->get##Type() == NullValue) \
      return s->replaceNode(node, firstChild, s->_curTree);


/*
 * Helper functions needed by simplifier handlers across projects
 */

void simplifyChildren(TR::Node * node, TR::Block * block, TR::Simplifier * s);
bool performTransformationSimplifier(TR::Node * node, TR::Simplifier * s);
void setIsHighWordZero(TR::Node * node, TR::Simplifier * s);
TR::Node *_gotoSimplifier(TR::Node * node, TR::Block * block,  TR::TreeTop* curTree,  TR::Optimization * s);
void foldIntConstant(TR::Node * node, int32_t value, TR::Simplifier * s, bool anchorChildrenP);
void foldUIntConstant(TR::Node * node, uint32_t value, TR::Simplifier * s, bool anchorChildrenP);
void foldLongIntConstant(TR::Node * node, int64_t value, TR::Simplifier * s, bool anchorChildrenP);
void foldFloatConstant(TR::Node * node, float value, TR::Simplifier * s);
void foldDoubleConstant(TR::Node * node, double value, TR::Simplifier * s);
void foldByteConstant(TR::Node * node, int8_t value, TR::Simplifier * s, bool anchorChildrenP);
void foldShortIntConstant(TR::Node * node, int16_t value, TR::Simplifier * s, bool anchorChildrenP);
bool swapChildren(TR::Node * node, TR::Node * & firstChild, TR::Node * & secondChild, TR::Simplifier * s);
bool isExprInvariant(TR_RegionStructure *region, TR::Node *node);
void orderChildren(TR::Node * node, TR::Node * & firstChild, TR::Node * & secondChild, TR::Simplifier * s);
TR::Node *foldRedundantAND(TR::Node * node, TR::ILOpCodes andOpCode, TR::ILOpCodes constOpCode, int64_t andVal, TR::Simplifier * s);
TR::Node* tryFoldAndWidened(TR::Simplifier* simplifier, TR::Node* node);
bool branchToFollowingBlock(TR::Node * node, TR::Block * block, TR::Compilation *comp);
void makeConstantTheRightChild(TR::Node * node, TR::Node * & firstChild, TR::Node * & secondChild, TR::Simplifier * s);
void makeConstantTheRightChildAndSetOpcode(TR::Node * node, TR::Node * & firstChild, TR::Node * & secondChild, TR::Simplifier * s);
TR::Node *replaceChild(int32_t childIndex, TR::Node* node, TR::Node* newChild, TR::Simplifier* s);
TR::Node *postWalkLowerTreeSimplifier(TR::TreeTop *tt, TR::Node *node, TR::Block *block, TR::Simplifier * s);
void foldFloatConstantEmulate(TR::Node * node, uint32_t value, TR::Simplifier * s);
void foldDoubleConstantEmulate(TR::Node * node, uint64_t value, TR::Simplifier * s);
bool isNaNFloat(TR::Node * node);
bool isNaNDouble(TR::Node * node);
bool isNZFloatPowerOfTwo(float value);
bool isNZDoublePowerOfTwo(double value);
bool isIntegralExponentInRange(TR::Node *parent, TR::Node *exponent, int64_t maxNegativeExponent, int64_t maxPositiveExponent, TR::Simplifier * s);
TR::Node *reduceExpTwoAndGreaterToMultiplication(int32_t exponentValue, TR::Node *baseNode, TR::ILOpCodes multOp, TR::Block *block, TR::Simplifier *s, int32_t maxExponent);
TR::Node *replaceExpWithMult(TR::Node *node,TR::Node *valueNode,TR::Node *exponentNode,TR::Block *block,TR::Simplifier *s);
bool decodeConversionOpcode(TR::ILOpCode op, TR::DataType nodeDataType, TR::DataType &sourceDataType, TR::DataType &targetDataType);
int32_t floatToInt(float value, bool roundUp);
int32_t doubleToInt(double value, bool roundUp);
void removePaddingNode(TR::Node *node, TR::Simplifier *s);
void stopUsingSingleNode(TR::Node *node, bool removePadding, TR::Simplifier *s);
TR::TreeTop *findTreeTop(TR::Node * callNode, TR::Block * block);
TR::Node *removeIfToFollowingBlock(TR::Node * node, TR::Block * block, TR::Simplifier * s);

#endif

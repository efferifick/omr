/*******************************************************************************
 *
 * (c) Copyright IBM Corp. 2015, 2016
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

#include "ddr/ir/ClassUDT.hpp"

ClassUDT::ClassUDT(size_t size, bool isClass, unsigned int lineNumber)
	: ClassType(isClass ? CLASS : STRUCT, size, lineNumber), _superClass(NULL), _isClass(isClass)
{
}

ClassUDT::~ClassUDT() {};

DDR_RC
ClassUDT::scanChildInfo(Scanner *scanner, void *data)
{
	return scanner->dispatchScanChildInfo(this, data);
}

string
ClassUDT::getSymbolKindName()
{
	return _isClass ? "class" : "struct";
}

DDR_RC
ClassUDT::enumerateType(BlobGenerator *blobGenerator, bool addFieldsOnly)
{
	return blobGenerator->dispatchEnumerateType(this, addFieldsOnly);
}

DDR_RC
ClassUDT::buildBlob(BlobGenerator *blobGenerator, bool addFieldsOnly, string prefix)
{
	return blobGenerator->dispatchBuildBlob(this, addFieldsOnly, prefix);
}

DDR_RC
ClassUDT::printToSuperset(SupersetGenerator *supersetGenerator, bool addFieldsOnly, string prefix = "")
{
	return supersetGenerator->dispatchPrintToSuperset(this, addFieldsOnly, prefix);
}

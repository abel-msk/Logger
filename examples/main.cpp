#include <iostream>
#include <stdio.h>
#include <string.h>
#include "Tokenizer.h"
#include "BinaryTree.h"
#include "BinaryTreeNode.h"
#include "SLinkedList.h"
#include "Logger.h"

class FilterElementTest {
    public:
    const char* key;
    const char* data;
    FilterElementTest(const char* k, const char* d);
};

FilterElementTest::FilterElementTest(const char* k, const char* d) 
{ 
    key=k; data=d;
};

class PrintAction: public BinaryTreeAction<long,FilterElementTest*> {
    virtual void run(long key, FilterElementTest* el) {
        std::cout << "Key="<< key <<", \tobjkey=" << el->key <<",\t objdata=" << el->data << std::endl;
    };
};


class frame {
    public:
    const char* str;
};

 
int main(void) {

    std::cout << "==============  Test Tokenizer ==============" << std::endl;

	frame *ctoken;
	const char* token;

	Tokenizer* tkz = new Tokenizer("%data% [%classname10%]:%level% Final text");

    token=tkz->getToken('%');

	SLinkedList<frame*> *sv = new SLinkedList<frame*>;

    tkz->reset();
	while ((token=tkz->getToken('%')) != 0) {
		ctoken = new frame;
		ctoken->str = token;
		sv->PushBack(ctoken);
        std::cout << "Process token=" << token << "|"<< std::endl;

	}

	if ( tkz->isParseComplete() ) {
		std::cout << "Parse complete" << std::endl;
	}
	else {
		std::cout << "Got an error" << std::endl;
	}

    SListIterator<frame*> it = sv->begin();

	while (it.Valid()) {
        ctoken  = it.Item();
		std::cout << "token=" << ctoken->str << std::endl;
        ++it;
	}
    
    std::cout << "==============  SVector delete ==============" << std::endl;
    delete sv;
    std::cout << "==============  Tokenizer clear ==============" << std::endl << std::flush;
    delete tkz;
    std::cout << "==============  Test 2 ==============" << std::endl;

    std::cout << "==============  Test Format ==============" << std::endl;

    LogFormat formatter;
    formatter.parseFmt("%date% [%levelL10%] %clname5%: %msg%");
    std::cout << "Parse. RC=" << formatter.hasError() << std::endl;

    std::cout << "==============  Test Filters ==============" << std::endl;
    FilterElementTest* tn1 = new FilterElementTest("a","a");
    tn1->key = "key";
    tn1->data = "data1";

    FilterElementTest* tn2 = new FilterElementTest("b","b");
    tn2->key = "key";
    tn2->data = "data2";

    // cmpStructType* c = new cmpStructType;

    // BinaryTreeNode<char,FilterElementTest,cmpStructType> node(tn1,new cmpStructType);
    // BinaryTreeNode<char,FilterElementTest,cmpStructType> node(tn1);

    BinaryTreeNode<long, FilterElementTest*,cmpLong>node(1,tn2);
    std::cout << "Compare result 2:"<< node.CompareTo(2) << std::endl;
    std::cout << "The key is:"<< node.getKey() << std::endl;
    std::cout << "The key is:"<< node.getValue()->data << std::endl;

    std::cout << "==============  Test 3 ==============" << std::endl;



    // BinaryTreeBase<long,FilterElementTest,cmpLong> store;
    BinaryTree<long,FilterElementTest*,cmpLong>* store  = new BinaryTree<long,FilterElementTest*,cmpLong>();


    store->add(80,new FilterElementTest("a","80"));
    store->add(50,new FilterElementTest("b","50"));
    store->add(100,new FilterElementTest("c","100"));
    store->add(20,new FilterElementTest("d","20"));
    store->add(70,new FilterElementTest("a1","70"));
    store->add(60,new FilterElementTest("a2","60"));
    store->add(55,new FilterElementTest("ff","55"));


    std::cout << "Object created" << std::endl;
    FilterElementTest* foundRes;


    // std::cout << (store->remove(50)?"REMOVED":"NOT REMOVED") << std::endl;


    foundRes = store->getByKey(50);
    std::cout << (foundRes!=0?foundRes->data:"NOT FOUND") << std::endl;


    foundRes = store->getByKey(55);
    std::cout << (foundRes!=0?foundRes->data:"NOT FOUND") << std::endl;
    foundRes = store->getByKey(60);
    std::cout << (foundRes!=0?foundRes->data:"NOT FOUND") << std::endl;
    foundRes = store->getByKey(70);
    std::cout << (foundRes!=0?foundRes->data:"NOT FOUND") << std::endl;
    foundRes = store->getByKey(20);
    std::cout << (foundRes!=0?foundRes->data:"NOT FOUND") << std::endl;
 

    std::cout << "==============  Test 4 ==============" << std::endl;
    foundRes = NULL;
    store->inOrderTraversal(new PrintAction());
    store->clear();
    delete store;

    std::cout << "==============  Test 5 ==============" << std::endl;

    Logger logprint("MAIN");

    logprint.error("ERROR string log out.");
    std::cout << "==============  Hide ERROR" << std::endl;
    logFactory.setFilter("MAIN",LogLevel::L_ERR,false);
    logprint.error("ERROR Hide string.");
    std::cout << "==============  Show DEBUG" << std::endl;
    logFactory.setFilter("MAIN",LogLevel::L_DEBUG,true);
    logprint.debug("Print string.");

    logprint.debug("Print DEBUG");
    logprint.info("Print INFO");
    std::cout << "==============  Finish ==============" << std::endl;


}


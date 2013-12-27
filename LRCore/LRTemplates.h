/*******************************************************************************
 LiquidReality engine common templates
 Nothing special but very useful
 (c) 2001 Maciej Mróz
 *******************************************************************************/

#ifndef _LIQUID_REALITY_TEMPLATES_H_
#define _LIQUID_REALITY_TEMPLATES_H_

#include"LRExceptionImpl.h"
#include<stdlib.h>
#include<windows.h>

namespace LR {

	//Generic list template - interface definition + some generic functions.
	//class T must have =,==,!= operators defined as well as copying constructor.
	//TODO: scrap this class!!!!!!
	template<class T> class LRTList {
	public:
		virtual void	addElement(const T &rfElement)=0;
		virtual void	insertElementAt(DWORD dwIndex,const T &rfElement)=0;
		virtual void	removeElementAt(DWORD dwIndex)=0;
		virtual void	setElementAt(DWORD dwIndex,const T &rfElement)=0;
		virtual DWORD	getElementCount() const =0;
		virtual void	clearList()=0;
		//utilities - all defined in terms of above functions
		
		bool isEmpty() {
			if(getElementCount()==0)
			{
				return true;
			}
			return false;
		}

		//user is responsible for deleting the copy
		T*		getElementCopyAt(DWORD dwIndex) {
			T*	pTmpItem=getElementPtrAt(dwIndex);
			T*	pOutItem=new T;
			*pOutItem=*pTmpItem;
			return pOutItem;
		}
		
		//slow operator= (n^2!!!)
		//should override in derived class for better efficiency
		//this operator is no longer virtual to get rid of annoying warnings
		//isuued by Intel C++ 6.0!!!
		void operator=(const LRTList &rfList) {
			if(!isEmpty()) clearList();
			DWORD	dwItemCount=rfList.getElementCount();
			DWORD	dwTmp=0;
			for(dwTmp=0;dwTmp<dwItemCount;dwTmp++) {
				addElement(*rfList.getElementPtrAt(dwTmp));
			}
		}

	};

	template<class T> class LRTLinkedListItem {
	public:
		T						m_Value;
		LRTLinkedListItem<T>	*m_pNextNode;
		LRTLinkedListItem<T>	*m_pPrevNode;
		//construction/destruction
		LRTLinkedListItem(const T &rfElement) {
			m_Value=rfElement;
			m_pNextNode=NULL;
			m_pPrevNode=NULL;
		}
	};

	//Linked list template - allows quite easy manipulation - doing stuff
	//like delete,add,remove,insert element. Of course slow random element
	//access.
	template<class T> class LRTLinkedList : public LRTList<T> {
	public:
		//direct use is possible but not neccesarily recommended :)))
		DWORD					m_dwItemCount;
		LRTLinkedListItem<T>	*m_pHeadNode;
		
		LRTLinkedList<T>() {
			m_dwItemCount=0;
			m_pHeadNode=NULL;
		}

		virtual ~LRTLinkedList<T>() {
			if(m_pHeadNode) {
				DWORD	dwTmp=0;
				LRTLinkedListItem<T>	*pTmpNode=m_pHeadNode;
				LRTLinkedListItem<T>	*pTmpNextNode=m_pHeadNode;
				while(pTmpNode) {
					if(dwTmp==m_dwItemCount) {
						throw InternalObjectCorruptionExceptionImpl("LRTLinked list destructor, too many nodes");
					}
					pTmpNextNode=pTmpNode->m_pNextNode;
					delete pTmpNode;
					pTmpNode=pTmpNextNode;
					dwTmp++;
				}
			}
			else {
				if(m_dwItemCount!=0) {
					throw InternalObjectCorruptionExceptionImpl("LRTLinked list destructor, inconsistent object state");
				}
			}
		}

		virtual void	addElement(const T &rfElement) {
			if(m_dwItemCount==0) {
				m_pHeadNode=new LRTLinkedListItem<T>(rfElement);
				m_dwItemCount++;
			}
			else {
				LRTLinkedListItem<T>	*pTempNode=m_pHeadNode;
				while(pTempNode->m_pNextNode) pTempNode=pTempNode->m_pNextNode;
				pTempNode->m_pNextNode=new LRTLinkedListItem<T>(rfElement);
				pTempNode->m_pNextNode->m_pPrevNode=pTempNode;
				m_dwItemCount++;
			}
		}

		virtual void	removeElementAt(DWORD dwIndex) {
			if(m_dwItemCount==0) return;
			if(dwIndex>=m_dwItemCount) throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,dwIndex);
			if(dwIndex==0) {
				if(m_dwItemCount==1) {
					if(!m_pHeadNode) {
						throw InternalObjectCorruptionExceptionImpl("LRTLinkedList::removeElementAt - inconsistent object state");
					}
					delete m_pHeadNode;
					m_pHeadNode=NULL;
					m_dwItemCount=0;
				}
				else {
					if(!m_pHeadNode) {
						throw InternalObjectCorruptionExceptionImpl("LRTLinkedList::removeElementAt - inconsistent object state");
					}
					LRTLinkedListItem<T>	*pTempHead=m_pHeadNode->m_pNextNode;
					delete m_pHeadNode;
					m_pHeadNode=pTempHead;
					m_pHeadNode->m_pPrevNode=NULL;
					m_dwItemCount--;
				}
			}
			else {
				LRTLinkedListItem<T>	*pTempNode=m_pHeadNode->m_pNextNode;
				LRTLinkedListItem<T>	*pPreviousNode=m_pHeadNode;
				DWORD					dwTemp;
				for(dwTemp=0;dwTemp<dwIndex;dwTemp++) {
					pPreviousNode=pTempNode;
					if(pTempNode->m_pNextNode) {
						pTempNode=pTempNode->m_pNextNode;
					}
					else {
						throw InternalObjectCorruptionExceptionImpl("LRTLinkedList::removeElementAt - unexpected NULL");
					}
				}
				//delete
				pPreviousNode->m_pNextNode=pTempNode->m_pNextNode;
				if(pTempNode->m_pNextNode) {
					pTempNode->m_pNextNode->m_pPrevNode=pPreviousNode;
				}
				delete pTempNode;
				m_dwItemCount--;
			}
		}

		virtual void insertElementAt(DWORD dwIndex,const T &rfElement) {
			if(dwIndex>m_dwItemCount) throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,dwIndex);
			if(dwIndex==0) {
				addElement(rfElement);
				return;
			}
			if(!m_pHeadNode)
				throw InternalObjectCorruptionExceptionImpl(
				"LRTLinkedList::insertElementAt - unconsistent object state");
			LRTLinkedListItem<T>	*pTempNode=m_pHeadNode;
			LRTLinkedListItem<T>	*pTempNode2=NULL;
			DWORD					dwTemp;
			for(dwTemp=1;dwTemp<dwIndex;dwTemp++) {
				if(pTempNode->m_pNextNode) {
					pTempNode=pTempNode->m_pNextNode;
				}
				else {
					throw InternalObjectCorruptionExceptionImpl("LRTLinkedList::insertElementAt - unexpected NULL");
				}
			}
			pTempNode2=new LRTLinkedListItem<T>(rfElement);
			pTempNode2->m_pNextNode=pTempNode->m_pNextNode;
			pTempNode->m_pNextNode->m_pPrevNode=pTempNode2;
			pTempNode2->m_pPrevNode=pTempNode;
			pTempNode->m_pNextNode=pTempNode2;
			m_dwItemCount++;
		}

		virtual void	setElementAt(DWORD dwIndex,const T &rfElement) {
			if(m_dwItemCount==0) return;
			if(dwIndex>=m_dwItemCount) throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,dwIndex);
			LRTLinkedListItem<T>	*pTempNode=m_pHeadNode;
			DWORD					dwTemp;
			for(dwTemp=0;dwTemp<dwIndex;dwTemp++) {
				if(pTempNode->m_pNextNode) {
					pTempNode=pTempNode->m_pNextNode;
				}
				else {
					throw InternalObjectCorruptionExceptionImpl("LRTLinkedList::setElementAt - unexpected NULL");
				}
			}
			pTempNode->m_Value=rfElement;
		}

		virtual DWORD	getElementCount() const {
			return m_dwItemCount;
		}

		virtual void	clearList() {
			if(m_pHeadNode) {
				DWORD	dwTmp=0;
				LRTLinkedListItem<T>	*pTmpNode=m_pHeadNode;
				LRTLinkedListItem<T>	*pTmpNextNode=m_pHeadNode;
				while(pTmpNode) {
					if(dwTmp==m_dwItemCount) {
						throw InternalObjectCorruptionExceptionImpl("LRTLinkedList::clearList - inconsistent object state");
					}
					pTmpNextNode=pTmpNode->m_pNextNode;
					delete pTmpNode;
					pTmpNode=pTmpNextNode;
					dwTmp++;
				}
			}
			else {
				if(m_dwItemCount!=0) {
					throw InternalObjectCorruptionExceptionImpl("LRTLinkedList::clearList - inconsistent object state");
				}
			}
		}

		void operator=(const LRTLinkedList<T> &rfList) {
			//more effective operator= than the default one
			clearList();
			if(rfList.m_dwItemCount) {
				LRTLinkedListItem<T>	*pTmpSrcNode=rfList.m_pHeadNode;
				LRTLinkedListItem<T>	*pTmpDstNode=NULL;
				if(pTmpSrcNode==NULL) {
					throw InternalObjectCorruptionException("LRTLinkedList::operator= - inconsistent source object state");
				}
				while(pTmpSrcNode) {
					if(m_pHeadNode==NULL) {
						pTmpDstNode=new LRTLinkedListItem<T>(pTmpSrcNode->m_Value);
						m_pHeadNode=pTmpDstNode;
						pTmpSrcNode=pTmpSrcNode->m_pNextNode;
						m_dwItemCount=1;
					}
					else {
						if(m_dwItemCount==rfList.m_dwItemCount) {
							throw InternalObjectCorruptionException("LRTLinkedList::operator= - source object broken (NULL expected)");
						}
						pTmpDstNode->m_pNextNode=new LRTLinkedListItem<T>(pTmpSrcNode->m_Value);
						pTmpSrcNode=pTmpSrcNode->m_pNextNode;
						pTmpDstNode=pTmpDstNode->m_pNextNode;
						m_dwItemCount++;
					}
				}
				if(m_dwItemCount!=rfList.m_dwItemCount) {
					throw InternalObjectCorruptionException("LRTLinkedList::operator= - operator failed for unknown reason");
				}
			}
		}
		//linked list specific methods
		virtual LRTLinkedListItem<T>* getListIterator() {
			return m_pHeadNode;
		}
		virtual T& getElement(LRTLinkedListItem<T> *iter) {
			return iter->m_Value;
		}
		virtual LRTLinkedListItem<T>* prevElement(LRTLinkedListItem<T> *iter) {
			return iter->m_pPrevNode;
		}
		virtual LRTLinkedListItem<T>* nextElement(LRTLinkedListItem<T> *iter) {
			return iter->m_pNextNode;
		}
		virtual LRTLinkedListItem<T>* removeElement(LRTLinkedListItem<T> *iter) {
			if(iter->m_pNextNode) {
				iter->m_pNextNode->m_pPrevNode=iter->m_pPrevNode;
				if(iter==m_pHeadNode) {
					m_pHeadNode=iter->m_pNextNode;
				}
				else {
					iter->m_pPrevNode->m_pNextNode=iter->m_pNextNode;
				}
				LRTLinkedListItem<T> *pNewIter=iter->m_pNextNode;
				delete iter;
				m_dwItemCount--;
				return pNewIter;
			}
			else { //possibly last one
				if(iter->m_pPrevNode) {
					iter->m_pPrevNode->m_pNextNode=iter->m_pNextNode;
					LRTLinkedListItem<T> *pNewIter=iter->m_pPrevNode;
					delete iter;
					m_dwItemCount--;
					return pNewIter;
				}
				else { //the only one
					delete iter;
					m_pHeadNode=NULL;
					m_dwItemCount--;
					return NULL;
				}
			}
		}
		virtual void detachElement(LRTLinkedListItem<T> *iter) {
			if(iter->m_pPrevNode) {
				iter->m_pPrevNode->m_pNextNode=iter->m_pNextNode;
			}
			if(iter->m_pNextNode) {
				iter->m_pNextNode->m_pPrevNode=iter->m_pPrevNode;
			}
			iter->m_pNextNode=NULL;
			iter->m_pPrevNode=NULL;
			m_dwItemCount--;
		}
		virtual void attachElement(LRTLinkedListItem<T> *iter) {
			if(m_pHeadNode) {
				iter->m_pNextNode=m_pHeadNode;
				m_pHeadNode->m_pPrevNode=iter;
				m_pHeadNode=iter;
				m_dwItemCount++;
			}
			else {
				m_pHeadNode=iter;
				m_dwItemCount++;
			}
		}
	};


	template<class T> void my_qsort(T* pTbl,int iStartElem,int iEndElem) {
		T	x;
		T tmp;
		int i,j;
		x=pTbl[iStartElem];
		i=iStartElem;
		j=iEndElem;
		do {
			while(x<pTbl[j]) j--;
			while(pTbl[i]<x) i++;
			if(i<=j) {
				tmp=pTbl[i];
				pTbl[i]=pTbl[j];
				pTbl[j]=tmp;
				i++;
				j--;
			}
		} while(i<j);
		if(iStartElem<j) my_qsort(pTbl,iStartElem,j);
		if(iEndElem>i) my_qsort(pTbl,i,iEndElem);
	};

	//Table list template. Allows very fast access to random elements
	//(just one pointer indirection) but some operations (insertion,
	//deletion) are potentially slow
	template<class T> class LRTTableList : public LRTList<T> {
	public:
		T		*m_pTbl;
		DWORD	m_dwItemCount;
		DWORD	m_dwTableSize;
		DWORD	m_dwAllocStep;

		LRTTableList(DWORD dwAllocStep=10) {
			m_pTbl=NULL;
			m_dwItemCount=0;
			m_dwTableSize=0;
			m_dwAllocStep=dwAllocStep;
		}

		virtual ~LRTTableList() {
			if(m_pTbl) {
				if(m_dwTableSize!=0) {
					delete [] m_pTbl;
					m_pTbl=NULL;
					m_dwTableSize=0;
					m_dwItemCount=0;
				}
				else {
					throw InternalObjectCorruptionExceptionImpl("LRTTableList destructor - inconsistent object state");
				}
			}
			else {
				if(m_dwItemCount!=0) {
					throw InternalObjectCorruptionExceptionImpl("LRTTableList destructor - inconsistent object state");
				}
			}
		}

		void setAllocStep(DWORD dwStep) {
			if(dwStep==0) return;
			m_dwAllocStep=dwStep;
		}

		//function overrides
		virtual void	addElement(const T &rfElement) {
			if(m_dwTableSize==m_dwItemCount) {
				if(m_dwItemCount==0) {
					m_pTbl=new T[m_dwAllocStep];
					m_dwTableSize=m_dwAllocStep;
					m_pTbl[m_dwItemCount++]=rfElement;
				}
				else {
	//				T		*pTmpTbl=new T[m_dwTableSize+m_dwAllocStep];
					T		*pTmpTbl=new T[m_dwTableSize*2];
					DWORD	dwTmp;
					for(dwTmp=0;dwTmp<m_dwItemCount;dwTmp++) pTmpTbl[dwTmp]=m_pTbl[dwTmp];
					delete [] m_pTbl;
					m_pTbl=pTmpTbl;
					m_pTbl[m_dwItemCount++]=rfElement;
	//				m_dwTableSize+=m_dwAllocStep;
					m_dwTableSize*=2;
				}
			}
			else {
				if(m_dwItemCount>m_dwTableSize) {
					throw InternalObjectCorruptionExceptionImpl("LRTTableList::addElement - inconsistent object state");
				}
				m_pTbl[m_dwItemCount++]=rfElement;
			}
		}

		virtual void	removeElementAt(DWORD dwIndex) {
			if(dwIndex>=m_dwItemCount) {
				throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,dwIndex);
			}
			if(dwIndex==(m_dwItemCount-1)) {
				m_dwItemCount--;
				return;
			}
			DWORD dwTmp;
			for(dwTmp=dwIndex;dwTmp<(m_dwItemCount-1);dwTmp++) {
				m_pTbl[dwTmp]=m_pTbl[dwTmp+1];
			}
			m_dwItemCount--;
		}

		virtual void	insertElementAt(DWORD dwIndex, const T &rfElement) {
			if(dwIndex>m_dwItemCount) {
				throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,dwIndex);
			}
			if(dwIndex==m_dwItemCount) {
				addElement(rfElement);
				return;
			}
			if(m_dwTableSize==m_dwItemCount) {
	//			T		*pTmpTbl=new T[m_dwTableSize+m_dwAllocStep];
				T		*pTmpTbl=new T[m_dwTableSize*2];
				DWORD	dwTmp;
				for(dwTmp=0;dwTmp<m_dwItemCount;dwTmp++) pTmpTbl[dwTmp]=m_pTbl[dwTmp];
				delete [] m_pTbl;
				m_pTbl=pTmpTbl;
	//			m_dwTableSize+=m_dwAllocStep;
				m_dwTableSize*=2;
			}
			DWORD	dwTmp;
			for(dwTmp=m_dwItemCount;dwTmp>dwIndex;dwTmp--) {
				m_pTbl[dwTmp]=m_pTbl[dwTmp-1];
			}
			m_pTbl[dwIndex]=rfElement;
			m_dwItemCount++;
		}

		virtual void	setElementAt(DWORD dwIndex,const T &rfElement) {
			if(dwIndex>=m_dwItemCount) {
				throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,dwIndex);
			}
			m_pTbl[dwIndex]=rfElement;
		}

		virtual DWORD	getElementCount() const {
			return m_dwItemCount;
		}

		virtual void	clearList() {
			if(m_pTbl) {
				if(m_dwTableSize!=0) {
					delete [] m_pTbl;
					m_pTbl=NULL;
					m_dwTableSize=0;
					m_dwItemCount=0;
				}
				else {
					throw InternalObjectCorruptionExceptionImpl("LRTTableList::clearList - inconsistent object state");
				}
			}
			else {
				if(m_dwItemCount!=0) {
					throw InternalObjectCorruptionExceptionImpl("LRTTableList::clearList - inconsistent object state");
				}
			}
		}

		void operator=(const LRTLinkedList<T> &rfList) {
			clearList();
			DWORD	dwCount=rfList.getElementCount();
			if(dwCount==0) return;
			m_pTbl=new T[dwCount];
			m_dwItemCount=dwCount;
			m_dwTableSize=dwCount;
			m_dwAllocStep= (m_dwAllocStep > 0) ? m_dwAllocStep : 5;
			DWORD	dwTmp=0;
			LRTLinkedListItem<T>	*pListItem=rfList.m_pHeadNode;
			for(dwTmp=0;dwTmp<m_dwItemCount;dwTmp++) {
				if(pListItem) {
					m_pTbl[dwTmp]=pListItem->m_Value;
					pListItem=pListItem->m_pNextNode;
				}
				else {
					clearList();
					throw LRInternalObjectCorruptionException("LRTTableList::operator=(1) - inconsistent source object state");
				}
			}
		}

		void operator=(const LRTTableList<T> &rfList) {
			clearList();
			DWORD	dwCount=rfList.getElementCount();
			if(dwCount==0) return;
			m_pTbl=new T[dwCount];
			m_dwItemCount=dwCount;
			m_dwTableSize=dwCount;
			m_dwAllocStep=rfList.m_dwAllocStep;
			DWORD dwTmp;
			for(dwTmp=0;dwTmp<m_dwItemCount;dwTmp++) {
				m_pTbl[dwTmp]=rfList.m_pTbl[dwTmp];
			}
		}

		T& operator[](DWORD dwIndex) {
			if(dwIndex<m_dwItemCount) return m_pTbl[dwIndex];
			else {
				throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,dwIndex);
			}
		}

		void sort() {
			if(m_dwItemCount>1) {
				my_qsort(m_pTbl,0,((int)m_dwItemCount)-1);
			}
		}

		int search(const T& rfElem) {
			if(!m_dwItemCount) return -1;
			int i=0;
			int j=((int)m_dwItemCount)-1;
			while(i<j) {
				if(m_pTbl[(i+j)/2]==rfElem) return (i+j)/2;
				else if(m_pTbl[(i+j)/2]<rfElem) i=(i+j+1)/2;
				else j=(i+j)/2;
			}
			if(m_pTbl[i]==rfElem) return i;
			return -1;
		}

		void exchange(DWORD dwIndex1,DWORD dwIndex2) {
			if((dwIndex1<m_dwItemCount) &&
				(dwIndex2<m_dwItemCount)) {
				T tTmp;
				tTmp=m_pTbl[dwIndex1];
				m_pTbl[dwIndex1]=m_pTbl[dwIndex2];
				m_pTbl[dwIndex2]=tTmp;
			}
			else {
				throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,
					(dwIndex1>dwIndex2) ? dwIndex1 : dwIndex2);
			}
		}

		//remove any 'cached' storage space
		void compact() {
			if(m_dwTableSize!=m_dwItemCount) {
				if(m_dwItemCount==0) {
					//simply delete everything
					delete [] m_pTbl;
					m_pTbl=NULL;
					m_dwTableSize=0;
				}
				else {
					//realloc
					T *pTmpTbl=new T[m_dwItemCount];
					DWORD dwTmp;
					for(dwTmp=0;dwTmp<m_dwItemCount;dwTmp++) pTmpTbl[dwTmp]=m_pTbl[dwTmp];
					delete [] m_pTbl;
					m_pTbl=pTmpTbl;
					m_dwTableSize=m_dwItemCount;
				}
			}
		}
	};

	template<class T> class LRTPtrTable : public LRTTableList<T> {
	public:
		LRTPtrTable(DWORD dwAllocStep=10) : LRTTableList<T>(dwAllocStep) {
		}
		virtual ~LRTPtrTable() {
			DWORD i;
			for(i=0;i<m_dwItemCount;i++) {
				delete m_pTbl[i];
			}
		}
	};

	template<class T> class LRTCOMPtrTable : public LRTTableList<T> {
	public:
		LRTCOMPtrTable(DWORD dwAllocStep=10) : LRTTableList<T>(dwAllocStep) {
		}
		virtual void	removeElementAt(DWORD dwIndex) {
			if(dwIndex>=m_dwItemCount) {
				throw IndexOutOfBoundsExceptionImpl(0,m_dwItemCount-1,dwIndex);
			}
			m_pTbl[dwIndex]->Release();
			LRTTableList<T>::removeElementAt(dwIndex);
		}
		~LRTCOMPtrTable() {
			DWORD i;
			for(i=0;i<m_dwItemCount;i++) {
				m_pTbl[i]->Release();
			}
		}
	};

	//similar to table list BUT geared rather at growable _structure_ arrays
	//anything that needs copying contructor other than memcpy cannot be used
	//with this template :)
	template<class T> class LRTStructureVector {
	public:
		T		*m_pTbl;
		DWORD	m_dwItemCount;
		DWORD	m_dwTableSize;
		DWORD	m_dwAllocStep;

		LRTStructureVector(DWORD dwAllocStep=10) {
			m_pTbl=NULL;
			m_dwItemCount=0;
			m_dwTableSize=0;
			m_dwAllocStep=dwAllocStep;
		}

		~LRTStructureVector() {
			if(m_pTbl) {
				if(m_dwTableSize!=0) {
					m_pTbl=NULL;
					m_dwTableSize=0;
					m_dwItemCount=0;
					delete [] m_pTbl;
				}
				else {
					throw LRInternalObjectCorruptionException("LRTStructureVector destructor - inconsistent object state");
				}
			}
			else {
				if(m_dwItemCount!=0) {
					throw LRInternalObjectCorruptionException("LRTStructureVector destructor - inconsistent object state");
				}
			}
		}
		//function overrides
		virtual void	addElement(const T* pElement) {
			if(m_dwTableSize==m_dwItemCount) {
				if(m_dwItemCount==0) {
					m_pTbl=new T[m_dwAllocStep];
					m_dwTableSize=m_dwAllocStep;
					memcpy(&m_pTbl[m_dwItemCount++],pElement,sizeof(T));
				}
				else {
					T		*pTmpTbl=new T[m_dwTableSize+m_dwAllocStep];
					DWORD	dwTmp;
					for(dwTmp=0;dwTmp<m_dwItemCount;dwTmp++) pTmpTbl[dwTmp]=m_pTbl[dwTmp];
					delete [] m_pTbl;
					m_pTbl=pTmpTbl;
					memcpy(&m_pTbl[m_dwItemCount++],pElement,sizeof(T));
					m_dwTableSize+=m_dwAllocStep;
				}
			}
			else {
				if(m_dwItemCount>m_dwTableSize) {
					throw LRInternalObjectCorruptionException("LRTTableList::addElement - inconsistent object state");
				}
				memcpy(&m_pTbl[m_dwItemCount++],pElement,sizeof(T));
			}
		}

		virtual T*		getElementPtrAt(DWORD dwIndex) const {
			if(dwIndex<m_dwItemCount) return &m_pTbl[dwIndex];
			else {
				throw IndexOutOfBoundsException(0,m_dwItemCount-1,dwIndex);
			}
		}

		virtual void	removeElementAt(DWORD dwIndex) {
			if(dwIndex>=m_dwItemCount) {
				throw IndexOutOfBoundsException(0,m_dwItemCount-1,dwIndex);
			}
			if(dwIndex==(m_dwItemCount-1)) {
				m_dwItemCount--;
				return;
			}
			DWORD dwTmp;
			for(dwTmp=dwIndex;dwTmp<(m_dwItemCount-1);dwTmp++) {
				memcpy(&m_pTbl[dwTmp],&m_pTbl[dwTmp+1],sizeof(T));
			}
			m_dwItemCount--;
		}

		virtual void	insertElementAt(DWORD dwIndex, const T* pElement) {
			if(dwIndex>m_dwItemCount) {
				throw IndexOutOfBoundsException(0,m_dwItemCount-1,dwIndex);
			}
			if(dwIndex==m_dwItemCount) {
				addElement(pElement);
				return;
			}
			if(m_dwTableSize==m_dwItemCount) {
				T		*pTmpTbl=new T[m_dwTableSize+m_dwAllocStep];
				DWORD	dwTmp;
				for(dwTmp=0;dwTmp<m_dwItemCount;dwTmp++) {
					memcpy(&pTmpTbl[dwTmp],&m_pTbl[dwTmp],sizeof(T));
				}
				delete [] m_pTbl;
				m_pTbl=pTmpTbl;
				m_dwTableSize+=m_dwAllocStep;
			}
			DWORD	dwTmp;
			for(dwTmp=m_dwItemCount;dwTmp>dwIndex;dwTmp--) {
				m_pTbl[dwTmp]=m_pTbl[dwTmp-1];
			}
			memcpy(&m_pTbl[dwIndex],pElement,sizeof(T));
			m_dwItemCount++;
		}
		virtual void	setElementAt(DWORD dwIndex,const T* pElement) {
			if(dwIndex>=m_dwItemCount) {
				throw IndexOutOfBoundsException(0,m_dwItemCount-1,dwIndex);
			}
			memcpy(&m_pTbl[dwIndex],pElement,sizeof(T));
		}
		virtual DWORD	getElementCount() const {
			return m_dwItemCount;
		}
		virtual void	clear() {
			if(m_pTbl) {
				if(m_dwTableSize!=0) {
					delete [] m_pTbl;
					m_pTbl=NULL;
					m_dwTableSize=0;
					m_dwItemCount=0;
				}
				else {
					throw LRInternalObjectCorruptionException("LRTStructureVector::clear - inconsistent object state");
				}
			}
			else {
				if(m_dwItemCount!=0) {
					throw LRInternalObjectCorruptionException("LRTStructureVector::clear - inconsistent object state");
				}
			}
		}
		T& operator[](DWORD dwIndex) {
			if(dwIndex<m_dwItemCount) return m_pTbl[dwIndex];
			else {
				throw IndexOutOfBoundsException(0,m_dwItemCount-1,dwIndex);
			}
		}
		T*	getTable() {
			return m_pTbl;
		}
	};

	template<class T> class LRTCOMPtr {
	private:
		T *m_pCOMInterface;
	public:
		LRTCOMPtr() {
			m_pCOMInterface=NULL;
		}
		~LRTCOMPtr() {
			if(m_pCOMInterface) {
				m_pCOMInterface->Release();
				m_pCOMInterface=NULL;
			}
		}
		LRTCOMPtr(T *pCOMInterface) {
			m_pCOMInterface=pCOMInterface;
		}
		LRTCOMPtr& operator=(const LRTCOMPtr& rfPtr) {
			if(m_pCOMInterface) {
				m_pCOMInterface->Release();
				m_pCOMInterface=NULL;
			}
			if(rfPtr.m_pCOMInterface) {
				m_pCOMInterface=rfPtr.m_pCOMInterface;
				m_pCOMInterface->AddRef();
			}
			return *this;
		}
		LRTCOMPtr& operator=(T *pCOMPtr) {
			if(m_pCOMInterface) {
				m_pCOMInterface->Release();
				m_pCOMInterface=NULL;
			}
			if(pCOMPtr) {
				m_pCOMInterface=pCOMPtr;
				m_pCOMInterface->AddRef();
			}
			return *this;
		}
		T*	operator->() {
			return m_pCOMInterface;
		}
	};

}

#endif
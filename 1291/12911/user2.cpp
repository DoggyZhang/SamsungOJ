#include<stdio.h>
#include<malloc.h>

#define MAX_PEOPLE   50000

typedef struct {
	int id;
	int tendency;
} Participant;

typedef struct NODE{
	Participant participant;
	NODE *pre;
	NODE *next;
} NODE, *PNODE;

PNODE mHead;
PNODE mTail;
PNODE mMid;
int mCount = 0;

int mLeftTotal = 0;
int mRightTotal = 0;

void freeNode( PNODE node ){
	if( node != NULL ){
		freeNode(node->next);
		node->pre = NULL;
		node->next = NULL;
		free(node);
		node = NULL;
	}
}

int compare( PNODE node1 , PNODE node2 ){
	if( node1->participant.tendency < node2->participant.tendency ){
		return -1;
	}else if( node1->participant.tendency > node2->participant.tendency ){
		return 1;
	}else{
		if( node1->participant.id < node2->participant.id ){
			return -1;
		}else if(node1->participant.id < node2->participant.id ){
			return 1;
		}else{
			return 0;
		}
	}
}

void init(int N, Participant participants[]) {
	mCount = 0;
	mMid = 0;
	mLeftTotal = 0;
	mRightTotal = 0;

	free(mHead);
	mHead = (PNODE)malloc(sizeof(NODE));
	mHead->next = NULL;
	mHead->pre = NULL;
	mTail = mHead;

	for( int n = 0; n < N ; n++){
		addParticipant( participants[n] );
	}
}

PNODE findIndex(Participant newParticipant){
	PNODE node = mHead;
	PNODE nextNode = mHead->next;
	while( nextNode != NULL ){
		if( nextNode->participant.tendency < newParticipant.tendency ){
			node = nextNode;
			nextNode = nextNode->next;
		}else if( nextNode->participant.tendency == newParticipant.tendency ){
			while( nextNode != NULL && nextNode->participant.id < newParticipant.tendency ){
				node = nextNode;
				nextNode = nextNode->next;
			}
			return node;
		}else{
			break;
		}
	}
	return node;
}

void addParticipant(Participant newParticipant) {
	PNODE node = findIndex(newParticipant);
	PNODE nextNode = node->next;

	PNODE newNode = (PNODE)malloc(sizeof(NODE));
	newNode->participant = newParticipant;
	newNode->pre = NULL;
	newNode->next = NULL;

	node->next = newNode;
	newNode->next = nextNode;

	newNode->pre = node;
	nextNode->pre = newNode;

	mCount ++;

	if( (mCount & 0x1) == 1 ){
		// 偶数->奇数
		int result = compare(newNode, mMid);
		if( result > 0 ){
			mLeftTotal = mLeftTotal - mMid->pre->participant.tendency + newParticipant.tendency;
		}else if( result < 0 ){
			mRightTotal = mRightTotal - mMid->participant.tendency + newParticipant.tendency;
			mMid = mMid->pre;
		}else{
			// 新加节点就是中点
		}
		//if( mMid > index ){
		//	mLeftTotal = mLeftTotal - mParticipants[mMid - 1].tendency + newParticipant.tendency;
		//}else if( mMid < index ){
		//	mRightTotal = mRightTotal - mParticipants[mMid].tendency + newParticipant.tendency;
		//}else{
		// 新加节点就是中点
		//}
	}else{
		// 奇数->偶数
		if( compare(newNode, mMid) >= 0){
			mLeftTotal += newParticipant.tendency;
			mRightTotal += mMid->participant.tendency;

			mMid = mMid->next;
		}else{
			mLeftTotal += mMid->participant.tendency;
			mRightTotal += newParticipant.tendency;
		}
		//if( mMid >= index ){
		//	mLeftTotal += newParticipant.tendency;
		//	mRightTotal += mParticipants[mMid].tendency;
		//}else{
		//	mLeftTotal += mParticipants[mMid].tendency;
		//	mRightTotal += newParticipant.tendency;
		//}
	}

	//for( int i = mCount ; i > index ; i-- ){
	//	mParticipants[i] = mParticipants[i - 1];
	//}
	//mParticipants[index] = newParticipant;
	//mMid = mCount >> 1;
}

int removeParticipants(int K) {
	if(mCount == 0){
		return 0;
	}
	int ids = 0;
	switch( K ){
	case 0: // Delete Min Participant
		PNODE first = mHead->next;
		ids = first->participant.id;
		
		mCount --;
		if( (mCount & 0x1) == 1 ){
			// 偶数->奇数
			mLeftTotal -= first->participant.tendency;
			mRightTotal -= mMid->participant.tendency;
		}else{
			// 奇数->偶数
			mLeftTotal -= first->participant.tendency;
			mLeftTotal += mMid->next.tendency;

			mMid = mMid->next;
		}
		break;
	case 1: // Delete Mid Participant
		if( mCount == 1 ){
			ids = mHead->next->participant.id;
			free(mHead->next);
			mHead->next = NULL;
			mCount = 0;
			mMid = 0;
			mLeftTotal = 0;
			mRightTotal = 0;
		}else if( mCount == 2 ){
			PNODE first = mHead->next;
			PNODE second = first->next;
			ids = first->participant.id + second->participant.id;
			freeNode(first);
			mHead->next = NULL;
			mCount = 0;
			mMid = 0;
			mLeftTotal = 0;
			mRightTotal = 0;
		}else{
			if( (mCount & 0x1) == 1 ){
				// 删除 1个
				ids = mMid->participant.id;
				mCount --;
				PNODE deleteNode = mMid;
				mMid = mMid->next;
				PNODE preNode = deleteNode->pre;
				PNODE nextNode = deleteNode->next;
				deleteNode->next = NULL;
				deleteNode->pre = NULL;
				preNode->next = nextNode;
				nextNode->pre = preNode;
			}else{
				// // 删除 2个
				PNODE deleteNode1 = mMid->pre;
				PNODE deleteNode2 = mMid;
				mMid = mMid->next;

				mLeftTotal -= deleteNode1->participant.tendency;
				mRightTotal -= deleteNode2->participant.tendency;

				ids = preMid->participant.id + mMid->participant.id;
				mCount = mCount - 2;

				PNODE preNode = deleteNode1->pre;
				PNODE nextNode = deleteNode2->next;

				deleteNode1->pre = NULL;
				deleteNode2->next = NULL;
				freeNode(deleteNode1);
				deleteNode1 = NULL;
				deleteNode2 = NULL;

				preNode->next = nextNode;
				nextNode->pre = preNode;
			}
		}
		break;
	case 2: // Delete Max Participant
		ids = mTail->participant.id;
		mCount --;

		mTail = mTail->pre;
		free(mTail->next);
		mTail->next = NULL;

		if( (mCount & 0x1) == 1 ){
			mRightTotal -= mParticipants[mCount - 1].tendency;
			mRightTotal += mMid->participant.tendency;

			mMid = mMid->pre;
		}else{
			mRightTotal -= mParticipants[mCount - 1].tendency;
			mLeftTotal -= mMid->pre->participant.tendency;
		}
		break;
	}

	return ids;
}

void getTotal(int tot[]) {
	tot[0] = mLeftTotal;
	tot[1] = mRightTotal;
}
/*
#include<stdio.h>
#include<malloc.h>

#define MAX_PEOPLE   50000

typedef struct {
	int id;
	int tendency;
} Participant;

Participant mParticipants[MAX_PEOPLE + 1];
int mParticipantsCount = 0;

int mMid = 0;
int mLeftTotal = 0;
int mRightTotal = 0;

int isSort(){
	for( int i = 0; i < mParticipantsCount - 1; i++ ){
		if( mParticipants[i].tendency > mParticipants[i + 1].tendency ){
			return i;
		}else if( mParticipants[i].tendency == mParticipants[i + 1].tendency ){
			if( mParticipants[i].id > mParticipants[i + 1].id){
				return i;
			}
		}
	}
	return -1;
}

int findIndex( Participant participant , int left , int right){
	if( left > right ){ 
		if( right < 0 ){
			return 0;
		}else if(left >= mParticipantsCount){
			return left;
		}else{
			if( mParticipants[left].tendency > participant.tendency ){
				return left;
			}else{
				return right;
			}
		}
	}
	int mid = (left + right) >> 1;
	if( mParticipants[mid].tendency < participant.tendency ){
		return findIndex(participant, mid + 1, right);
	}else if( mParticipants[mid].tendency > participant.tendency ){
		return findIndex(participant, left, mid - 1);
	}else{
		if( mParticipants[mid].id > participant.id ){
			int next = mid;
			while( next >= 0 
				&& mParticipants[next].tendency == participant.tendency 
				&& mParticipants[next].id > participant.id ){
				next--;
			}
			return next + 1;
		}else if( mParticipants[mid].id < participant.id ){
			int next = mid;
			while( next < mParticipantsCount 
				&& mParticipants[next].tendency == participant.tendency 
				&& mParticipants[next].id < participant.id ){
				next++;
			}
			return next;
		}else{
			return mid;
		}
	}
}

void addParticipant(Participant newParticipant) {
	if( newParticipant.id == 64065 && newParticipant.tendency == 72622
		|| newParticipant.id == 52255 && newParticipant.tendency == 72622){
			printf("");
	}
	int index = findIndex(newParticipant , 0 , mParticipantsCount - 1);
	if( (mParticipantsCount & 0x1) == 1 ){
		// 奇数->偶数
		if( mMid >= index ){
			mLeftTotal += newParticipant.tendency;
			mRightTotal += mParticipants[mMid].tendency;
		}else{
			mLeftTotal += mParticipants[mMid].tendency;
			mRightTotal += newParticipant.tendency;
		}
	}else{
		// 偶数->奇数
		if( mMid > index ){
			mLeftTotal = mLeftTotal - mParticipants[mMid - 1].tendency + newParticipant.tendency;
		}else if( mMid < index ){
			mRightTotal = mRightTotal - mParticipants[mMid].tendency + newParticipant.tendency;
		}else{
			// 新加节点就是中点
		}
	}

	for( int i = mParticipantsCount ; i > index ; i-- ){
		mParticipants[i] = mParticipants[i - 1];
	}
	mParticipants[index] = newParticipant;
	mParticipantsCount ++;
	mMid = mParticipantsCount >> 1;

	int sort = isSort();
	if( sort != -1 ){
		//printf("  Not Sort: %d \n" , sort);
	}
}

void init(int N, Participant participants[]) {
	mParticipantsCount = 0;
	mMid = 0;
	mLeftTotal = 0;
	mRightTotal = 0;

	for( int n = 0; n < N ; n++){
		addParticipant( participants[n] );
	}

	int sort = isSort();
	if( sort != -1 ){
		//printf("  Not Sort: %d \n" , sort);
	}
}

int removeParticipants(int K) {
	if(mParticipantsCount == 0){
		return 0;
	}
	int ids = 0;
	switch( K ){
	case 0: // Delete Min Participant
		ids = mParticipants[0].id;
		if( (mParticipantsCount & 0x1) == 1 ){
			mLeftTotal -= mParticipants[0].tendency;
			mLeftTotal += mParticipants[mMid].tendency;
		}else{
			mLeftTotal -= mParticipants[0].tendency;
			mRightTotal -= mParticipants[mMid].tendency;
		}

		for( int i = 0; i <= mParticipantsCount - 1 ; i++ ){
			mParticipants[i] = mParticipants[i + 1];
		}
		mParticipantsCount --;
		mMid = mParticipantsCount >> 1;
		break;
	case 1: // Delete Mid Participant
		if( mParticipantsCount == 1 ){
			ids = mParticipants[0].id;
			mParticipantsCount = 0;
			mMid = 0;
			mLeftTotal = 0;
			mRightTotal = 0;
		}else if( mParticipantsCount == 2 ){
			ids = mParticipants[0].id + mParticipants[1].id;
			mParticipantsCount = 0;
			mMid = 0;
			mLeftTotal = 0;
			mRightTotal = 0;
		}else{
			if( (mParticipantsCount & 0x1) == 1 ){
				ids = mParticipants[mMid].id;
				for( int i = mMid ; i <= mParticipantsCount - 1 ; i++ ){
					mParticipants[i] = mParticipants[i + 1];
				}
				mParticipantsCount --;
			}else{
				mLeftTotal -= mParticipants[mMid - 1].tendency;
				mRightTotal -= mParticipants[mMid].tendency;

				ids = mParticipants[mMid - 1].id + mParticipants[mMid].id;
				for( int i = mMid - 1; i <= mParticipantsCount - 2; i++ ){
					mParticipants[i] = mParticipants[i + 2];
				}
				mParticipantsCount = mParticipantsCount - 2;
			}

			mMid = mParticipantsCount >> 1;
		}
		break;
	case 2: // Delete Max Participant
		ids = mParticipants[mParticipantsCount - 1].id;
		if( (mParticipantsCount & 0x1) == 1 ){
			mRightTotal -= mParticipants[mParticipantsCount - 1].tendency;
			mRightTotal += mParticipants[mMid].tendency;
		}else{
			mRightTotal -= mParticipants[mParticipantsCount - 1].tendency;
			mLeftTotal -= mParticipants[mMid - 1].tendency;
		}

		mParticipantsCount --;
		mMid = mParticipantsCount >> 1;
		break;
	}

	int sort = isSort();
	if( sort != -1 ){
		//printf("  Not Sort: %d \n" , sort);
	}

	return ids;
}

void getTotal(int tot[]) {
	tot[0] = mLeftTotal;
	tot[1] = mRightTotal;
}*/
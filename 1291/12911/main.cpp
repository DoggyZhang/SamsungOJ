#ifndef _CRT_SECURE_NO_WARNINGS 
#define _CRT_SECURE_NO_WARNINGS 
#endif

#include<stdio.h>

#define MAXID 100000
#define MAXTENDENCY 100001

#define REMOVERATE 6
#define AVERAGERATE 4

typedef struct {
	int id;
	int tendency;
} Participant;

extern void init(int N, Participant mParticipants[]);
extern void addParticipant(Participant mParticipant);
extern int removeParticipants(int K);
extern void getTotal(int tot[]);

static int seed = 3;
static int N, M;

static int idChk[MAXID];
static int accepted;

static bool conChk;

static int pseudo_rand(void)
{
	seed = seed * 431345 + 2531999;
	return seed & 0x7FFFFFFF;
}

static int getTendency() {
	int tendency;

	do {
		tendency = pseudo_rand() % MAXTENDENCY;
	} while (tendency == MAXTENDENCY - 1);

	return tendency;
}

static void initFunc(int test_case, int n) {

	Participant participants[50001];

	for (int i = 0; i < n; ++i) {
		int id = pseudo_rand() % MAXID;
		int tendency = getTendency();

		while (idChk[id] == test_case) {
			id = (id + 1) % MAXID;
		}

		idChk[id] = test_case;

		participants[i].id = id;
		participants[i].tendency = tendency;
	}

	init(n, participants);
}

static void addFunc(int test_case) {

	Participant participant;

	int id = pseudo_rand() % MAXID;
	int tendency = getTendency();

	while (idChk[id] == test_case) {
		id = (id + 1) % MAXID;
	}

	idChk[id] = test_case;

	participant.id = id;
	participant.tendency = tendency;

	addParticipant(participant);
}

static int run(int test_case) {
	
	accepted = 0;

	bool conChk = true;

	initFunc(test_case, N);

	while (M--) {
		int command = pseudo_rand() % (REMOVERATE + AVERAGERATE);

		if (conChk && command < AVERAGERATE) {
			int tot[2];
			int correctTot[2];

			getTotal(tot);

			scanf("%d%d", &correctTot[0], &correctTot[1]);

			if (tot[0] == correctTot[0] && tot[1] == correctTot[1]) {
				++accepted;
			}else{
				printf(" GetTotal Error \n");
				printf("   Error : left: %d , right: %d\n" , tot[0] , tot[1]);
				printf("   Right : left: %d , right: %d\n" , correctTot[0] , correctTot[1]);
			}

			conChk = false;
		}
		else {
			int K = pseudo_rand() % 3;
			int retId, correctId;

			retId = removeParticipants(K);

			scanf("%d", &correctId);

			if (retId == correctId) {
				++accepted;
			}else{
				printf(" RemoveParticipants Error \n");
				printf("   Error : K: %d\n" , K);
				printf("   Error : retId: %d\n" , retId);
				printf("   Right : CorrectId: %d \n" , correctId);
			}

			addFunc(test_case);
			if (K == 1 && N % 2 == 0) {
				addFunc(test_case);
			}

			conChk = true;
		}
	}

	return accepted;
}

int main(void) {
	int T;
	int correct, totalScore = 0;

	freopen("sample_input.txt", "r", stdin);

	scanf("%d", &T);

	setbuf(stdout, NULL);

	for (int test_case = 1; test_case <= T; ++test_case) {
		printf("-------------------------------------\n");
		printf("Case %d : \n", test_case);

		scanf("%d%d%d%d", &seed, &N, &M, &correct);
		if (run(test_case) == correct) {
			printf("#%d 100\n", test_case);
			totalScore += 100;
		}
		else {
			printf("#%d 0\n", test_case);
		}
	}

	printf("Total Score = %d\n", totalScore / T);
	while(1);
	return 0;
}
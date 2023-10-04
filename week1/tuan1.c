#include<stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
//dinh nghia kieu du lieu student
struct User{
	int status; // 1:active, 0:block
	char username[20];
	char password[10];
	struct User *next;
};

// dinh nghia link list
struct LinkList{
	struct User *head;
	struct User *tail;
};

// khoi tao danh sach lien ket trong
void initializeList(struct LinkList *list){
	list->head = NULL;
	list->tail = NULL;
}

void addUser(struct LinkList *list, char username[20],char password[],int status ){
	struct User *new_user = (struct User*)malloc(sizeof(struct User));
	strcpy(new_user->username,username);
	strcpy(new_user->password, password);
	new_user->status = status;
	new_user->next = NULL;
	
	if(list->tail == NULL){  // list khong co phan tu nao
		list->head = new_user;
		list->tail = new_user;
	}else{
		list->tail->next = new_user;
		list->tail = new_user;
	}
}

void readFile(char filename[], struct LinkList *list){
	FILE *fptr;
	fptr = fopen(filename, "r");
	if(fptr == NULL){
		printf("ERROR!");
		exit(1);
	}
	
	while(!feof(fptr)){
		char username[20];
		char password[10];
		int status;
		fscanf(fptr,"%s\t%s\t%d\n",username,password, &status);
		addUser(list,username,password,status);
	}
	fclose(fptr);
}

void writeFile(char filename[], struct LinkList *list, struct User* user){
	FILE *fptr;
	fptr = fopen(filename, "a");
	if(fptr == NULL){
		printf("ERROR!\n");
		exit(1);
	}
	fprintf(fptr,"%s %s %d",user->username, user->password,user->status);
   	fclose(fptr);
}

void updateFile(char filename[], struct LinkList *list){
	FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        printf("Error!\n");
        exit(1);
    }

	struct User* current = list->head;
	while(current != NULL ){
		fprintf(fptr,"%s %s %d\n",current->username, current->password,current->status);
		current = current->next;
	}
    fclose(fptr);
}

void menu(){
	printf("\nUSER MANAGEMENT PROGRAM\n");
	printf("-----------------\n");
	printf("1.Register\n");
	printf("2.Sign In\n");
	printf("3.Search\n");
	printf("4.Sign Out\n");
}

bool checkExistUser(char username[20],struct LinkList *list){
	struct User* current = list->head;
	while(current != NULL){
		if(strcmp(current->username,username) == 0) return false;  // truong hop da ton tai ten tai khoan thif tra ve false
		current = current->next;
	}
	return true;
}

void registerUser(struct LinkList *list, char filename[]) {
    char username[20];
    char password[10]; 

    printf("Register new account\n");
    printf("Username: ");
    scanf("%s", username);
    printf("Password: ");
    scanf("%s", password);

    if (!checkExistUser(username, list)) {
        printf("Username has already existed\n");
    } else {
        addUser(list, username, password, 1); 
        struct User* newUser = list->tail;
        writeFile(filename, list, newUser); 
        printf("Successful registration\n");
    }
}


struct User* search(struct LinkList *list , char account[20]){
	struct User* current = list->head;
	while(current != NULL){
		if(strcmp(current->username,account) == 0){
			return current;	
		}
		current = current->next;
	}
	return NULL;		
}

int main(){
	int choice;
	struct LinkList list;
	initializeList(&list);
	char filename[] = "account.txt";
	readFile(filename,&list);
	
	bool isLoggedIn = false;
	char currentUsername[20];
	int counter;
	do{
		menu();
		printf("Your choice (1-4, other to quit):");
		scanf("%d",&choice);
		switch(choice){
			case 1:
				registerUser(&list, filename);
				break;
			case 2:
				counter = 1;
				bool loginSuccess = false;
				while(!(loginSuccess == true || counter > 3)){
					char username[20];
					char password[10];
					printf("Sign in to your account\n");
				    printf("Username: ");
				    scanf("%s", username);
				    printf("Password: ");
				    scanf("%s", password);
				    
					struct User* current = list.head;
					while(!(current == NULL || loginSuccess == true)){
						if(strcmp(current->username,username) == 0){
							if(current->status ==0){
								printf("Your account had been blocked\n");
								current= NULL;
							}
							else {
								if(strcmp(current->password,password) == 0 ){
									printf("Hello %s\n",current->username);
									loginSuccess = true;
									isLoggedIn = true;
									strcpy(currentUsername, current->username);
									current = current->next;
								}else{
									printf("Your password is incorrect\n");
									counter++;
									if(counter >3){
										printf("Your account has been blocked!\n");
										current->status = 0;
										updateFile(filename, &list);
									}
									current = current->next;
								}
							}
						}
						else{	
							if(current->next == NULL){
								printf("Your username or your password is incorrect!\n");
							}
							current = current->next;
						}
					}		
				}
				break;
			case 3:
				if (isLoggedIn == false){
					printf("Please sign in to continue!\n");
				}else{
					char accountName[20];
					printf("Input account you want to search: ");
					scanf("%s",accountName);
					
					struct User* account = search(&list, accountName);
					if(account != NULL){
						if(account->status ==0){
							printf("Account is blocked\n");
						}else{
							printf("Account is active\n");
						}
					}else{
						printf("Cannot find account\n");
					}
				}
				break;
			case 4:
				if(isLoggedIn == false){
					printf("Please sign in to continue!\n");
				}else{	
					char username[20];
					printf("Input your account to sign out:\n");
					printf("---Username: ");
					scanf("%s",username);
					if(strcmp(currentUsername, username) != 0){
						printf("Cannot find account\n");
					}else{
						isLoggedIn = false;
						printf("Goodbye %s!",username);
					}
				}
				break;
			default:
				return 0;
		}
				
	}while(choice >=1 && choice <= 4);
	return 0;
}


/*
void signIn(struct LinkList *list, char filename[], bool isLoggedIn){
	int counter =1;
	bool loginSuccess = false;
	while(!(loginSuccess == true || counter > 3)){
		char username[20];
		char password[10];
		printf("Sign in to your account\n");
	    printf("Username: ");
	    scanf("%s", username);
	    printf("Password: ");
	    scanf("%s", password);
	    
		struct User* current = list->head;
		while(!(current == NULL || loginSuccess == true)){
			if(strcmp(current->username,username) == 0){
				if(current->status ==0){
					printf("Your account had been blocked\n");
				}
				else {
					if(strcmp(current->password,password) == 0 ){
						printf("Hello %s\n",current->username);
						loginSuccess = true;
						isLoggedIn = true;
						current = current->next;
					}else{
						printf("Your password is incorrect\n");
						counter++;
						if(counter >3){
							printf("Your account has been blocked!\n");
							current->status = 0;
							updateFile(filename, list);
						}
						current = current->next;
					}
				}
			}
			else{	
				if(current->next == NULL){
					printf("Your username or your password is incorrect!\n");
				}
				current = current->next;
			}
		}		
	}
}
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lists.h"

/* Add a group with name group_name to the group_list referred to by 
* group_list_ptr. The groups are ordered by the time that the group was 
* added to the list with new groups added to the end of the list.
*
* Returns 0 on success and -1 if a group with this name already exists.
*
* (I.e, allocate and initialize a Group struct, and insert it
* into the group_list. Note that the head of the group list might change
* which is why the first argument is a double pointer.) 
*/
int add_group(Group **group_list_ptr, const char *group_name) {
    // First, check if the group already exists
    if ( find_group(*group_list_ptr, group_name) ) {
        return -1;      //group already exists.
    } else {
        //group doesn't exist, proceed to make one.
        // First, create a new pointer to a Group
        Group *newGrp = malloc(sizeof(Group)); // use malloc memory allocation

        if ( newGrp == NULL ) { //print error and exit
            printf("Error when creating group pointer. Program will now exit. \n");
            exit(0);
        } else {
            int LENGTH = strlen(group_name) + 1;
            newGrp->name = malloc(LENGTH); //assign group name from the passed in param
            // since name is char, we only need # of bytes = length of string + 1;

            if ( newGrp->name == NULL ) {
                printf("Error when adding group name. Program will now exit \n");
                exit(0);
            } else {
                strncpy(newGrp->name, group_name, LENGTH ); // assign group_name as name of newGrp
                newGrp->name[LENGTH] = '\0'; // add the terminating character
                newGrp->next = NULL; // assign the next to NULL to indicate end of list.
            }
        }

        // now that you've created your group and assigned it the name, time to add it to the group list.
        // if group list is empty, just point the group list ptr to our newly created group.
        if ( *group_list_ptr == NULL ) {
            *group_list_ptr = newGrp;
            return 0;
        } else { // else traverse to the end of the list and add our new group there.
            Group *currentGrp = *group_list_ptr; // create a new pointer for easy traversing
            while ( currentGrp->next ) {
                currentGrp = currentGrp->next;
            }
            // Finally at the end of the list, on the last element.
            // Add the our newly created group to the end of the group list.
            currentGrp->next = newGrp;
            return 0;
        }
    }
}

/* Print to standard output the names of all groups in group_list, one name
*  per line. Output is in the same order as group_list.
*/
void list_groups(Group *group_list) {

    Group *currentGrp = group_list; // get a current ptr for easy traversal

    if ( currentGrp ) { // if groups exist, proceed
        while ( currentGrp->next ) { // go through & print all grps that have a "next"
            printf("%s \n", currentGrp->name);
            currentGrp = currentGrp->next;
        }
        printf("%s \n", currentGrp->name); // lastly, print the last grp (the one without next)
    } else printf(" \n");   // if no groups exist yet, print blank line
}

/* Search the list of groups for a group with matching group_name
* If group_name is not found, return NULL, otherwise return a pointer to the 
* matching group list node.
*/
Group *find_group(Group *group_list, const char *group_name) {
    Group *currentGrp = group_list;

    if ( currentGrp == NULL ) return NULL;  // if group list is empty

    // iterate through the group list and look for group_name
    while( currentGrp->next ) {
    // compare current_grp name string to group_name, if same, return the current pointer
        if ( strcmp(currentGrp->name, group_name) == 0 ) {
            return currentGrp;
        }
        currentGrp = currentGrp->next;
    }

    // if not found in the previous elements, check the last element of group list
    if( strcmp( currentGrp->name, group_name ) == 0 ) {
        return currentGrp;
    } else return NULL; // if group name not found

}

/* Add a new user with the specified user name to the specified group. Return zero
* on success and -1 if the group already has a user with that name.
* (allocate and initialize a User data structure and insert it into the
* appropriate group list)
*/
int add_user(Group *group, const char *user_name) {
    // ASSUMPTION : group exists
    // check if user already exists in the group
    if ( find_prev_user(group, user_name) != NULL ) {   // if user_name already in group, return -1
        return -1;
    } else {
        // if the user_name is not already in the group, then add new user to the beginning of group user list
        // First, let's make a new user
        User *newUsr = malloc(sizeof(User));
        if ( newUsr == NULL ) {  //if error, exit
            printf("Error while creating a new user. Program will now exit. \n");
            exit(0);
        } else {
            int LENGTH = strlen(user_name) + 1;
            newUsr->name = malloc(LENGTH);

            if ( newUsr->name == NULL ) {
                printf("Error while creating user name. Program will now exit. \n");
                exit(0);
            } else { // assign the new user name
                strncpy(newUsr->name, user_name, LENGTH ); // assign user_name as name of newUsr
                newUsr->name[LENGTH] = '\0';
                newUsr->balance = 0; // assign the initial user balance to 0
                newUsr->next = NULL; // assign next to NULL to indicate end of list.
            }
        }

        // Now that we've made a user, it's time to add it to the given group_name
        if ( group->users == NULL ) { // if group is empty, assign user and exit
            group->users = newUsr;
        } else {
            newUsr->next = group->users;    // since lowest paying user first, add newUsr to beginning of list
            group->users = newUsr;
        }
        return 0;   // successfully add, return 0;
    }
}

/* Remove the user with matching user and group name and
* remove all her transactions from the transaction list. 
* Return 0 on success, and -1 if no matching user exists.
* Remember to free memory no longer needed.
* (Wait on implementing the removal of the user's transactions until you 
* get to Part III below, when you will implement transactions.)
*/
int remove_user(Group *group, const char *user_name) {
    User *currentUser = group->users;
    User *prevUser = NULL;

    if ( currentUser ) {    // if the group has any users
        prevUser = find_prev_user(group, user_name);
        if ( prevUser ) {
            if ( strcmp(prevUser->name, user_name) == 0 ) {
                currentUser = prevUser; // the first user = user_name
                if ( currentUser->next != NULL ) { // if more than one user in the group
                    group->users = currentUser->next; // redirect head to user #2 (next)
                } else {
                    group->users = NULL;    // if current user is only one, assign list to NULL
                }
            } else {
                currentUser = prevUser->next;
                prevUser->next = currentUser->next; // redirect prev pointer to next
            }
            currentUser->next = NULL;  // break link with rest of the list
            free(currentUser);  // free memory
            remove_xct(group, user_name); // remove all transactions associated with this user
            return 0;
        } else return -1; // user not in group
    } else return -1;
}

/* Print to standard output the names of all the users in group, one
* per line, and in the order that users are stored in the list, namely 
* lowest payer first.
*/
void list_users(Group *group) {
    // ASSUMPTION : group exists (since buxfer checks for group before calling this func )
    User *userPtr = group->users; // list of all users

    if ( userPtr ) {    // if 1 or more users exist
        while ( userPtr->next ) { // iterate through user list, print out all users
            printf("%s \n", userPtr->name);
            userPtr = userPtr->next;
        }

        printf("%s \n", userPtr->name); // last user
    } else printf(" \n");   // if no users, print blank line
}

/* Print to standard output the balance of the specified user. Return 0
* on success, or -1 if the user with the given name is not in the group.
*/
int user_balance(Group *group, const char *user_name) {

    User *currentUser = group->users;   // pointer to user group user list
    User *prevUser = NULL;  // pointer for previous user

    if( currentUser ) {  // if user list is not empty
        prevUser = find_prev_user(group, user_name);
        if ( prevUser ) {
            // if this user is in the group
            if ( strcmp(prevUser->name, user_name) == 0 ) {  // if first user is user_name
                // then get this user balance
                printf("$%.2f\n", prevUser->balance);
                return 0;
            } else {
                // get the next user balance
                printf("$%.2f\n", prevUser->next->balance);
                return 0;
            }
        } else return -1;   // user not in this group.
    } else return -1;   // user list is empty, user does not exist in this group.
}

/* Print to standard output the name of the user who has paid the least 
* If there are several users with equal least amounts, all names are output. 
* Returns 0 on success, and -1 if the list of users is empty.
* (This should be easy, since your list is sorted by balance). 
*/
int under_paid(Group *group) {
    // ASSUMPTION : group exists ( since buxfer checks for group before calling this )
    User *currentUser = group->users;
    if ( currentUser ) {
        User *underPaid = currentUser;  // assign "underPaid" pointer to currentUser (#1 which is always the lowest)
        while ( currentUser->next ) {
            /*
             * Iterate through the list, comparing each user to the underPaid one.
             * If the user balance of current user is less than or equal to that of underPaid, then
             * it gets printed to screen.
             */
            if ( currentUser->balance <= underPaid->balance ) {
                // if the currentUser balance is <= to the underpaid balance, then print to screen
                printf("%s\n", currentUser->name);
            }
            currentUser = currentUser->next; // go on to next one w/o exiting because we want ALL under_paid
        }

        if ( currentUser->balance <= underPaid->balance ) { // check the last element
            printf("%s\n", currentUser->name );
        }
        return 0;   // successful exit
    } else return -1;  // no users in this group
}

/* Return a pointer to the user prior to the one in group with user_name. If 
* the matching user is the first in the list (i.e. there is no prior user in 
* the list), return a pointer to the matching user itself. If no matching user 
* exists, return NULL. 
*
* The reason for returning the prior user is that returning the matching user 
* itself does not allow us to change the user that occurs before the
* matching user, and some of the functions you will implement require that
* we be able to do this.
*/
User *find_prev_user(Group *group, const char *user_name) {

    User *previous, *current; // pointers for previous and current users

    if ( group->users ) { // if there are users in the group
        current = group->users; // assign current to the head of the group user list
        // case 1 : 1 user
        if( current->next == NULL ){
            if ( strcmp(current->name, user_name) == 0 ) {
                return current;
            } else return NULL;
        } else {
            // case 2 : multiple users
            // Check current (head = first user)
            if ( strcmp(current->name, user_name) == 0 ) {
                return current;     // if first user = user_name, return the first element itself.
            } else {
                // if it's not the first one, proceed to further users
                previous = current;
                current = current->next;
                while ( current->next ) {
                    if ( strcmp(current->name, user_name) == 0 ) {  // if the current = user_name
                        return previous;    // return the pointer to the previous user
                    }
                    previous = current; // reassign previous and current, continue to iterate
                    current = current->next;
                }

                if(strcmp(current->name, user_name) == 0) { // check the last element
                    return previous;
                }
            }
            return NULL; // if user not found in the group, return NULL.
        }
    } else return NULL; // if no users in the group, return NULL.
}

/*
 * Meant to be used inside add_xct, with it's parameters.
 * This bit of code makes a new transaction, then adds it to the head of the xcts struct
*/

void _xct_helper (Group *group, const char *user_name, double amount) {
    Xct *newTrans = malloc(sizeof(Xct));    // make new transaction
    if ( newTrans != NULL ) {
        int LENGTH = strlen(user_name) + 1;
        newTrans->name = malloc(LENGTH); // make new transaction name element
        if ( newTrans->name != NULL ) {
            strncpy(newTrans->name, user_name, LENGTH ); // assign user_name as name of transaction
            newTrans->name[LENGTH] = '\0';
            newTrans->amount = amount;  // assign amount to the transaction
        } else {
            printf("Error while making new transaction name.\n");
            exit(0);
        }

        if (  group->xcts == NULL ) {   // if no transactions, just assign it to transaction
            group->xcts = newTrans;
        } else {
             // add transaction to head to it will be easier to retrieve for "recent_xct" function
            newTrans->next = group->xcts;
            group->xcts = newTrans;
        }
    } else {
        printf("Error while making a new transaction. \n");
        exit(0);
    }
}

/*
*   This is a helper function for add transaction. It updates the user position in the user list,
*   rearranging the list in an ascending order, so that the lowest paying users are at the beginning.
*   return 0 = success; return -1 = fail.
*   @param user -- this is a pointer to user to be repositioned.
*   @param group -- a pointer to your group (in which the user is).
*/

int _update_user_position(Group *group, User *user){

    // make new placeholder pointers to be used here
    User *prevUser;
    User *currentUser = group->users; //get the head

    if ( user == NULL ) { // if invalid user input
        return -1;
    }

    // if only one user in list, exit successfully
    if ( currentUser->next == NULL ) {
        return 0;
    } else {
        /* IF multiple users in list:
        * 1) Remove user from list by reassigning previous and next pointers.
        * 2) Reinsert the user at a new position
        */
        prevUser = find_prev_user(group, user->name);

        if ( strcmp(prevUser->name, user->name) == 0 ) {  // if first user
            group->users = user->next; // redirect head to #2 (next)
            user->next = NULL;  // break link with rest of the list
        } else {    // if middle user
            prevUser->next = user->next;    // redirect previous pointer to next
            user->next = NULL;  // break link with next/rest of the list
        }

        currentUser = group->users;

        while ( currentUser->next ) {
            /*
                Iterate through the list until you arrive at a place where our user's balance
                if LESS or EQUAL TO than the current user's balance. At this point, get the
                prevUser of currentUser, assign prevUser.next pointer to our user.
                Then assign our user.next pointer to currentUser, thereby inserting our
                user into it's correct place in the user list.
            */
            if ( user->balance < currentUser->balance ) {
                prevUser = find_prev_user(group, currentUser->name);    // find previous user
                prevUser->next = user;    // redirect it's pointer to our user
                user->next = currentUser; // point our user's next to current user
                return 0;   // exit successfully
            }
            currentUser = currentUser->next;
        }

        // if not in above, check if the last currentUser balance in list is greater than our user balance
        if ( user->balance < currentUser->balance ) {
            prevUser = find_prev_user(group, currentUser->name);
            prevUser->next = user;
            user->next = currentUser;
            return 0;
        }

        // if our user balance is greater than the entire list, place it at the end and exit
        currentUser->next = user;
        return 0;
    }
}

/* Add the transaction represented by user_name and amount to the appropriate 
* transaction list, and update the balances of the corresponding user and group. 
* Note that updating a user's balance might require the user to be moved to a
* different position in the list to keep the list in sorted order. Returns 0 on
* success, and -1 if the specified user does not exist.
*/
int add_xct(Group *group, const char *user_name, double amount) {
    User *prevUser = NULL; // pointer for previous user
    User *user = NULL; // pointer to user associated with this transaction

    // Check if user exists
    prevUser = find_prev_user(group, user_name);

    if ( prevUser ) {   // if user in group
        if ( strcmp(prevUser->name, user_name) == 0 ) {
            user = prevUser;
        } else {
            user = prevUser->next;
        }
        _xct_helper(group, user_name, amount);  // add the transaction to xct node
        user->balance = user->balance + amount; // update user balance
        _update_user_position(group, user);     // update user position based on new balance
        return 0;
    } else return -1; // user does not exist in this group
}

/* Print to standard output the num_xct most recent transactions for the 
* specified group (or fewer transactions if there are less than num_xct 
* transactions posted for this group). The output should have one line per 
* transaction that prints the name and the amount of the transaction. If 
* there are no transactions, this function will print nothing.
*/
void recent_xct(Group *group, long nu_xct) {
    // ASSUMPTION : group exists

    Xct *xctPtr = group->xcts;

    if( xctPtr == NULL ) {
        printf(" \n"); //print nothing if no transactions
    } else {
        long num;
        for ( num = nu_xct ; num > 0; num-- ) {
            printf("Transaction #%s; Amount: %.2f.\n", xctPtr->name, xctPtr->amount);
            if( xctPtr->next == NULL ) {
                break;
            }
            xctPtr = xctPtr->next;
         }
    }
}

/* Remove all transactions that belong to the user_name from the group's 
* transaction list. This helper function should be called by remove_user. 
* If there are no transactions for this user, the function should do nothing.
* Remember to free memory no longer needed.
*/

void remove_xct(Group *group, const char *user_name) {
    Xct *currentXct = group->xcts;  // make your pointers
    Xct *prevXct = NULL;

    if ( currentXct ) { // if there are any transactions
        while ( currentXct->next ) {
            // iterate through xcts to find ones associated with given user_name
            // then get the previous pointer and point it to then next,
            // free up memory and then go on down the list. Continue till last element.
            if ( strcmp(currentXct->name, user_name) == 0 ) {
                if ( prevXct == NULL  ) {
                    group->xcts = currentXct->next;
                } else {
                    prevXct->next = currentXct->next;
                }
                //currentXct->next = NULL;
                free(currentXct);
            }
            prevXct = currentXct;
            currentXct = currentXct->next;
        }

        if ( strcmp(currentXct->name, user_name) == 0 ) {
            if ( prevXct == NULL ) {    // if this is the only transaction
                group->xcts = NULL;
            } else {    // if this is the last transaction
                free(currentXct);
                prevXct->next = NULL;
            }
        }
    }
}
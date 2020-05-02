#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
// Add your system includes here.

#include "ftree.h"


/*
 * Returns the FTree rooted at the path fname.
 *
 * Use the following if the file fname doesn't exist and return NULL:
 * fprintf(stderr, "The path (%s) does not point to an existing entry!\n", fname);
 *
 */

void generate_path(const char *fname,char* path,char* new_path)
{
    strcpy(new_path,path);
    strcat(new_path,fname);
    strcat(new_path,"/");
}

struct TreeNode* initial_node(const char *fname,char* path)
{
    struct TreeNode* root = malloc(sizeof(struct TreeNode));
    
    if(root==NULL)
    {
        perror("malloc");
        exit(1);
    }

    root->fname = malloc((1+strlen(fname))*sizeof(char));
    
    if(root->fname==NULL)
    {
        perror("malloc");
        exit(1);
    }
    
    strcpy(root->fname,fname);
    
    struct stat s_buf;
    
    
    int len = strlen(fname)+strlen(path);
    char newpath[len+2];
    
    generate_path(fname,path,newpath);
    newpath[strlen(newpath) - 1] = '\0';
    
    if(lstat(newpath,&s_buf)!=0 )
    {
        fprintf(stderr, "The path (%s) does not point to an existing entry!\n", fname);
        return NULL;
    }
    
    root->permissions = s_buf.st_mode & 0777;
    
    if(S_ISDIR(s_buf.st_mode)) root->type = 'd';
    else if(S_ISREG(s_buf.st_mode)) root->type = '-';
    else if(S_ISLNK(s_buf.st_mode)) root->type = 'l';
    
    root->next=NULL;
    root->contents=NULL;
    
    return root;
}

struct TreeNode* constructTree(const char *fname,char* path)
{
    int len = strlen(fname) + strlen(path);
    char current_path[len+2];
    generate_path(fname,path,current_path);
    
    DIR *new_d_ptr = opendir(current_path);
    
    if (new_d_ptr == NULL)
    {
      perror("opendir");
      exit(1);
    }
    
    struct dirent *entry_ptr;
    entry_ptr = readdir(new_d_ptr);
    
    struct TreeNode* root = NULL;
    struct TreeNode* tmp = NULL;
    struct TreeNode* node = NULL;
    
    
    while(entry_ptr)
    {
        if (entry_ptr->d_name[0] != '.') {
            node=initial_node(entry_ptr->d_name,current_path);
            
            if(node->type=='d') node->contents=constructTree(node->fname,current_path);
            
            if(root==NULL)
            {
                root=node;
                tmp=root;
            }
            
            else tmp->next=node;
        
            tmp = node;
        }
        entry_ptr = readdir(new_d_ptr);
    }
    
    if(closedir(new_d_ptr)==-1)
    {
      perror("closedir");
      exit(1);
    }
    return root;

}

struct TreeNode *generate_ftree(const char *fname) {
    struct stat s_buf;
    if(lstat(fname,&s_buf)==-1)
    {
      fprintf(stderr, "The path (%s) does not point to an existing entry!\n", fname);
      return NULL;
    }
    
    int len = strlen(fname) + strlen("");
    
    char path[len+2];
    generate_path(fname,"",path);
    
    struct TreeNode* root = initial_node(fname,"");
    
    if(root->type=='d') root->contents=constructTree(fname,"");
    
    return root;
}


/*
 * Prints the TreeNodes encountered on a preorder traversal of an FTree.
 *
 * The only print statements that you may use in this function are:
 * printf("===== %s (%c%o) =====\n", root->fname, root->type, root->permissions)
 * printf("%s (%c%o)\n", root->fname, root->type, root->permissions)
 *
 */

void print_ftree(struct TreeNode *root) {
    static int depth = 0;
    printf("%*s", depth * 2, "");
    
    if(root->type=='d')
    {
        printf("===== %s (%c%o) =====\n", root->fname, root->type, root->permissions);
        depth++;
        if(root->contents!=NULL) print_ftree(root->contents);
        depth--;
    }
    
    else printf("%s (%c%o)\n", root->fname, root->type, root->permissions);
    
    if(root->next!=NULL) print_ftree(root->next);
}


/*
 * Deallocate all dynamically-allocated memory in the FTree rooted at node.
 *
 */
void deallocate_ftree (struct TreeNode *node) {
   if(node==NULL) return;
   deallocate_ftree(node->contents);
   deallocate_ftree(node->next);
   free(node->fname);
   free(node);
}

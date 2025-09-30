#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>


void trim(char* _buf)
{
    int src = 0, dst = 0;
    while (_buf[src])
    {
        if (!isspace((unsigned char)_buf[src]))
            _buf[dst++] = _buf[src];
        src++;
    }
    _buf[dst] = '\0';
}

typedef struct _tagNode tNode;

typedef struct _tagStack
{
    tNode* pNodeArr[1000];
    int  iCount;
} tStack;

void stack_push(tStack* _stack, tNode* _pNode)
{
    _stack->pNodeArr[_stack->iCount] = _pNode;
    ++_stack->iCount;
}

void stack_pop(tStack* _stack)
{
    if (0 == _stack->iCount)
    {
        return;
    }
    else
    {
        --_stack->iCount;
    }
}

tNode* stack_top(tStack* _stack)
{
    if (0 == _stack->iCount)
    {
        return NULL;
    }
    else
    {
        return _stack->pNodeArr[_stack->iCount - 1];
    }
}

bool stack_empty(tStack* _stack)
{
    if (0 == _stack->iCount)
    {
        return true;
    }
    else
    {
        return false;
    }
}

typedef struct _tagNode
{
    char             cData;
    struct _tagNode* pLeftChild;
    struct _tagNode* pRightChild;
} tNode;

tNode* create_node(char _cData) {
    tNode* newNode = (tNode*)malloc(sizeof(tNode));
    newNode->cData = _cData;
    newNode->pLeftChild = NULL;
    newNode->pRightChild = NULL;
    return newNode;
}

tNode* build_tree(char* _buf)
{
    tNode* pRootNode = NULL;
    tStack stack = { 0 };

    for (size_t i = 0; i < strlen(_buf); ++i)
    {

        if ('(' == _buf[i])
        {
            stack_push(&stack, NULL);
        }
        else if (isalpha(_buf[i]))
        {
            tNode* newNode = create_node(_buf[i]);
            if (!pRootNode)
            {
                pRootNode = newNode;
            }
            stack_push(&stack, newNode);
        }
        else if (')' == _buf[i])
        {
            tNode* pOlder = NULL;
            tNode* pYounger = NULL;

            while (!stack_empty(&stack))
            {
                tNode* culNode = stack_top(&stack);
                stack_pop(&stack);

                // '(' 만났으면 탈출
                if (!culNode)
                {
                    break;
                }
                else
                {
                    if (!pOlder)
                    {
                        pOlder = culNode;
                    }
                    else
                    {
                        pYounger = culNode;
                    }
                }
            }

            tNode* pParent = stack_top(&stack);

            if (pParent)
            {
                if (!pYounger)
                {
                    pParent->pLeftChild = pOlder;
                }
                else
                {
                    pParent->pLeftChild = pYounger;
                    pParent->pRightChild = pOlder;
                }

            }

        }
    }

    return pRootNode;
}

void release_tree(tNode* _culNode)
{
    if (!_culNode)
    {
        return;
    }
    else
    {
        release_tree(_culNode->pLeftChild);
        release_tree(_culNode->pRightChild);
        free(_culNode);
    }

}

void pre_order(tNode* _pNode)
{
    // NULL이면 반환
    if (!_pNode)
    {
        return;
    }

    tStack stack = { 0 };
    stack_push(&stack, _pNode);

	printf("pre-order: ");

    while (!stack_empty(&stack))
    {
        tNode* culNode = stack_top(&stack);
        stack_pop(&stack);

        printf("%c", culNode->cData);
		printf(" ");

        //오른쪽 자식이 있다면 먼저 푸쉬
        if (culNode->pRightChild)
        {
            stack_push(&stack, culNode->pRightChild);

        }

        if (culNode->pLeftChild)
        {
            stack_push(&stack, culNode->pLeftChild);
        }
    }
	printf("\n");
}


void in_order(tNode* _pNode)
{
    if (!_pNode)
    {
        return;
    }

    tStack stack = { 0 };
    tNode* curNode = _pNode;

    printf("in-order: ");

    while (curNode || !stack_empty(&stack))
    {
        // 왼쪽 끝까지 내려가면서 스택에 push
        while (curNode)
        {
            stack_push(&stack, curNode);
            curNode = curNode->pLeftChild;
        }

        // 왼쪽 자식이 없는 노드를 만나면 방문
        curNode = stack_top(&stack);
        stack_pop(&stack);
        printf("%c", curNode->cData);
		printf(" ");

        // 오른쪽 자식으로 이동
        curNode = curNode->pRightChild;
    }
	printf("\n");
}

void post_order(tNode* _pRootNode)
{
    if (!_pRootNode)
    {
        return;
    }

    tStack stack = { 0 };
    tNode* lastVisited = NULL;
    tNode* curNode = _pRootNode;

	printf("post-order: ");

    while (curNode || !stack_empty(&stack))
    {
        // 왼쪽 끝까지 내려가면서 스택에 push
        while (curNode)
        {
            stack_push(&stack, curNode);
            curNode = curNode->pLeftChild;
        }

        // 스택에서 top 확인
        tNode* peekNode = stack_top(&stack);

        // 오른쪽 자식이 없거나 이미 방문했으면 출력
        if (!peekNode->pRightChild || peekNode->pRightChild == lastVisited)
        {
            printf("%c", peekNode->cData);
			printf(" ");
           
            lastVisited = peekNode;
            stack_pop(&stack);
        }
        else
        {
            // 오른쪽 자식으로 이동
            curNode = peekNode->pRightChild;
        }

    }
	printf("\n");
}

void tree_array(tNode* _culNode, char* _cTreeArr, int _idx)
{
    if (!_culNode || _idx >= 1000)
    {
        return;
    }

    _cTreeArr[_idx] = _culNode->cData;

    tree_array(_culNode->pLeftChild, _cTreeArr, _idx * 2);
    tree_array(_culNode->pRightChild, _cTreeArr, _idx * 2 + 1);
}



int main()
{
    char buf[1000];

    scanf("%999[^\n]", buf);

    trim(buf);

    tNode* root = build_tree(buf);

    //배열트리 구현
	char cTreeArr[1000] = { 0 };
	tree_array(root, cTreeArr, 1); 
    
    // 순회 구현
    pre_order(root);
    
    in_order(root);
    
    post_order(root);
    
   
    
    release_tree(root);



    return 0;
}                



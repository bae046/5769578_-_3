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

void pre_order(tNode* _pNode, char* _Arr)
{
    // NULL이면 반환
    if (!_pNode)
    {
        return;
    }

    tStack stack = { 0 };
    stack_push(&stack, _pNode);

    int idx = 0;

    while (!stack_empty(&stack))
    {
        tNode* culNode = stack_top(&stack);
        stack_pop(&stack);

        _Arr[idx] = culNode->cData;
        ++idx;

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
}

void in_order(tNode* _pNode, char* _Arr)
{
    if (!_pNode)
    {
        return;
    }

    tStack stack = { 0 };
    tNode* curNode = _pNode;

    int idx = 0;

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
        _Arr[idx] = curNode->cData;
        ++idx;

        // 오른쪽 자식있으면 반복
        curNode = curNode->pRightChild;


    }
}

void post_order(tNode* _pNode, char* _Arr)
{
    if (!_pNode)
    {
        return;
    }

    tStack stack = { 0 };
    tNode* lastVisited = NULL;
    tNode* curNode = _pNode;

    int idx = 0;

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
            _Arr[idx] = peekNode->cData;
            ++idx;
            lastVisited = peekNode;
            stack_pop(&stack);
        }
        else
        {
            // 오른쪽 자식으로 이동
            curNode = peekNode->pRightChild;
        }

    }
}

void iter(char* _CArr) 
{
    for (size_t i = 0; i < strlen(_CArr); ++i)
    {
        printf("%c ", _CArr[i]);
    }
    printf("\n");
}



int main()
{
    char buf[1000];

    scanf("%999[^\n]", buf);

    trim(buf);

    tNode* root = build_tree(buf);

    char preOrderArr[1000] = { 0 };
    pre_order(root, preOrderArr);
    printf("pre-order: ");
    iter(preOrderArr);

    char inOrderArr[1000] = { 0 };
    in_order(root, inOrderArr);
    printf("in-order: ");
	iter(inOrderArr);

    char postOrderArr[1000] = { 0 };
    post_order(root, postOrderArr);
    printf("post-order: ");
    iter(postOrderArr);


    return 0;
}


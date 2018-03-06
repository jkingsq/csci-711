#include <stdlib.h>
#include "Figure.h"
#include "Solid.h"

SolidBucket solidBucketLast(SolidBucket bucket) {
    SolidBucket last = bucket;
    while(last->next != NULL) {
        last = last->next;
    }
    return last;
}

SolidBucket solidBucketPush(SolidBucket bucket, Solid solid) {
    SolidBucket new = malloc(sizeof(struct st_solidbucket));
    new->solid = solid;
    if(bucket == NULL) {
        new->next = NULL;
        return new;
    }
    new->next = bucket;
    return new;
}

int solidBucketLength(SolidBucket bucket) {
    int result = 0;
    while(bucket != NULL){
        result++;
        bucket = bucket->next;
    }
    return result;
}

Solid solidBucketPop(SolidBucket *bucket) {
    if (bucket == NULL){
        fprintf(stderr, "solidBucketPop received a null pointer!");
    } else if (*bucket == NULL) {
        fprintf(stderr, "solidBucketPop received an empty bucket!");
    }
    SolidBucket resultNode = *bucket;
    Solid result = resultNode->solid;
    *bucket = (*bucket)->next;
    free(resultNode);
    return result;
}

void SolidBucketDestroy(SolidBucket bucket) {
    while (bucket != NULL) {
        SolidBucket next = bucket->next;
        free(bucket);
        bucket = next;
    }
}

// returns a new list containing all of the solids that intersect the given
// bounding box.
SolidBucket solidBucketFilterInside(SolidBucket bucket, AABB inside) {
    SolidBucket iterator = bucket, result;
    while(iterator != NULL) {
        Solid s = bucket->solid;
        if(aabbIntersect(s.figure.aabb, inside))
            result = solidBucketPush(result, s);
    }
    return result;
}

SolidBucket solidBucketAppend(SolidBucket a, SolidBucket b) {
    if(a == NULL)
        return b;
    solidBucketLast(a)->next = b;
    return a;
}

SolidBucket solidBucketSort(SolidBucket bucket) {
    //base cases
    if (bucket == NULL || bucket->next == NULL)
        return bucket;
    Solid pivot = solidBucketPop(&bucket);
    SolidBucket less = NULL;
    SolidBucket greater = NULL;
    while (bucket != NULL) {
        Solid b = solidBucketPop(&bucket);
        if (b.figure.aabb.maxZ <= pivot.figure.aabb.maxZ)
            less = solidBucketPush(less, b);
        else 
            greater = solidBucketPush(greater, b);
    }
    less = solidBucketSort(less);
    greater = solidBucketSort(greater);
    return solidBucketAppend(greater, solidBucketPush(less, pivot));
}

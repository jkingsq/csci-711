#include <stdlib.h>
#include "Solid.h"

int aabbIntersect(AABB a, AABB b) {
    if(a.minX > b.maxX || a.maxX > b.minX)
        return 0;
    if(a.minX > b.maxY || a.maxY > b.minY)
        return 0;
    if(a.minZ > b.maxZ || a.maxZ > b.minZ)
        return 0;
    return 1;
}

SolidBucket solidBucketLast(SolidBucket bucket) {
    SolidBucket last = bucket;
    while(last->next != NULL) {
        last = last->next;
    }
    return last;
}

SolidBucket solidBucketInsert(SolidBucket bucket, Solid solid) {
    SolidBucket new = malloc(sizeof(struct st_solidbucket));
    if(bucket == NULL)
        return new;
    SolidBucket last = solidBucketLast(bucket);
    return bucket;
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
        return NULL;
    }
    SolidBucket resultNode = *bucket;
    Solid result = resultNode->solid;
    *bucket = (*bucket)->next;
    free(resultNode);
    return result->solid;
}

SolidBucket solidBucketCopy(SolidBucket bucket) {
    
}

void SolidBucketDestroy(SolidBucket bucket) {
    while (list != NULL) {
        EdgeBucket next = list->next;
        free(list);
        list = next;
    }
}

// returns a new list containing all of the solids that intersect the given
// bounding box.
SolidBucket solidBucketFilterInside(SolidBucket bucket, AABB inside) {
    SolidBucket iterator = bucket, result;
    while(iterator != NULL) {
        Solid s = bucket->solid;
        if(aabbIntersect(s.boundingBox, inside)
            result = solidBucketInsert(result, s);
    }
    return result;
}

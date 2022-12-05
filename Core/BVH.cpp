
#include "BVH.h"

BVHNode* BVH::recursive_build(std::vector<Object*> objs) {
    BVHNode* node = new BVHNode();
    Bound bound;
    for (int i = 0; i < objs.size(); ++i) {
        bound = union_bounds(bound, objs[i]->get_bound());
    }
    if (objs.size() == 1){
        node->object = objs[0];
        node->bounds = objs[0]->get_bound();
        node->left = nullptr;
        node->right = nullptr;
        return node;
    }else if(objects.size() == 2){
        node->left = recursive_build(std::vector<Object*> {objects[0]});
        node->right = recursive_build(std::vector<Object*> {objects[1]});
    }else{
        Bound centroidBounds;
        for (int i = 0; i < objs.size(); ++i) {
            centroidBounds = union_bounds(centroidBounds, objs[i]->get_bound());
        }
        int axis = centroidBounds.max_extent();
        switch (axis) {
            case 0:
                std::sort(objects.begin(), objects.end(), [](Object* f1, Object* f2) {
                    return f1->get_bound().get_centroid().x < f2->get_bound().get_centroid().x;
                });
                break;
            case 1:
                std::sort(objects.begin(), objects.end(), [](Object* f1, Object* f2) {
                    return f1->get_bound().get_centroid().y < f2->get_bound().get_centroid().y;
                });
                break;
            case 2:
                std::sort(objects.begin(), objects.end(), [](Object* f1, Object* f2) {
                    return f1->get_bound().get_centroid().z < f2->get_bound().get_centroid().z;
                });
                break;
        }

        auto begin = objs.begin();
        auto middle = objs.begin() + (objs.size() / 2);
        auto end = objs.end();

        auto leftObjects = std::vector<Object*>(begin, middle);
        auto rightObjects = std::vector<Object*>(middle, end);
        assert(objects.size() == (leftObjects.size() + rightObjects.size()));

        node->left = recursive_build(leftObjects);
        node->right = recursive_build(rightObjects);
        node->bounds = union_bounds(node->left->bounds, node->right->bounds);

        return node;
    }

    return nullptr;
}

Intersection BVH::intersect(const Ray& ray) const {
    Intersection intersection;
    if (!root) return intersection;
    intersection = get_intersection(root, ray);
    return intersection;
}

Intersection BVH::get_intersection(BVHNode* node, const Ray& ray) const {
    Intersection intersection;
    if (!node->bounds.intersect(ray)) return intersection;

    if (node->left == nullptr && node->right == nullptr){
        intersection = node->object->get_intersection(ray);
        return intersection;
    }

    auto left = get_intersection(node->left, ray);
    auto right = get_intersection(node->right, ray);
    if (left.distance < right.distance) return left;
    return right;
}

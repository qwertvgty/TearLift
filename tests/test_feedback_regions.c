#include <assert.h>
#include <stdio.h>
#include "../src/gfx/vkb.h"

int main(void) {
    VkImageCopy r[64];
    vkb_image im = {.width = 17, .height = 9, .depth = 1, .layers = 6, .levels = 3, .aspect = VK_IMAGE_ASPECT_COLOR_BIT};
    assert(vkb_feedback_regions(NULL, r, 64) == 0);
    assert(vkb_feedback_regions(&im, NULL, 64) == 0);
    assert(vkb_feedback_regions(&im, r, 64) == 3);
    assert(r[0].extent.width == 17 && r[1].extent.width == 8 && r[2].extent.height == 2);
    assert(r[2].srcSubresource.mipLevel == 2 && r[2].dstSubresource.layerCount == 6);
    assert(r[0].srcSubresource.aspectMask == VK_IMAGE_ASPECT_COLOR_BIT);
    assert(vkb_feedback_regions(&im, r, 2) == 0);
    im.aspect = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
    assert(vkb_feedback_regions(&im, r, 64) == 6);
    assert(r[0].srcSubresource.aspectMask == VK_IMAGE_ASPECT_DEPTH_BIT);
    assert(r[3].srcSubresource.aspectMask == VK_IMAGE_ASPECT_STENCIL_BIT);
    im.aspect = VK_IMAGE_ASPECT_DEPTH_BIT; im.layers = 1; im.depth = 8;
    assert(vkb_feedback_regions(&im, r, 64) == 3 && r[2].extent.depth == 2);
    im.width = im.height = im.depth = 1;
    assert(vkb_feedback_regions(&im, r, 64) == 3 && r[2].extent.width == 1);
    im.levels = 0;
    assert(vkb_feedback_regions(&im, r, 64) == 0);
    im.levels = 33;
    assert(vkb_feedback_regions(&im, r, 64) == 0);
    im.levels = 1; im.aspect = 0;
    assert(vkb_feedback_regions(&im, r, 64) == 0);
    im.aspect = VK_IMAGE_ASPECT_COLOR_BIT | VK_IMAGE_ASPECT_DEPTH_BIT;
    assert(vkb_feedback_regions(&im, r, 64) == 0);
    puts("Feedback copy regions: all checks passed");
    return 0;
}

/* Copyright (c) 2023, Canaan Bright Sight Co., Ltd
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SELF_LEARNING_H_
#define _SELF_LEARNING_H_

#include "utils.h"
#include "ai_base.h"

typedef struct Evec
{
    string category;
    float score; 
    string bin_file;
    
} Evec;

/**
 * @brief 自学习类
 * 主要封装了对于每一帧图片，从预处理、运行到后处理给出结果的过程
 */
class SelfLearning : public AIBase
{
    public:

        /** 
        * for video
        * @brief SelfLearning 构造函数，加载kmodel,并初始化kmodel输入、输出
        * @param kmodel_file kmodel文件路径
        * @param crop_wh     剪切范围
        * @param topk        识别范围
        * @param isp_shape   isp输入大小（chw）
        * @param vaddr       isp对应虚拟地址
        * @param paddr       isp对应物理地址
        * @param debug_mode 0（不调试）、 1（只显示时间）、2（显示所有打印信息）
        * @return None
        */
        SelfLearning(const char *kmodel_file, FrameSize crop_wh, float thres, int topk, FrameCHWSize isp_shape, uintptr_t vaddr, uintptr_t paddr, const int debug_mode);
       
        /** 
        * @brief  SelfLearning 析构函数
        * @return None
        */
        ~SelfLearning();

        /**
         * @brief 视频流预处理（ai2d for video）
         * @return None
         */
        void pre_process();


        /**
         * @brief kmodel推理
         * @return None
         */
        void inference();

        /**
         * @brief 获取kmodel的输出
         * @return None
         */
        float* get_kpu_output(int *out_len);

        /** 
        * @brief postprocess 函数
        * @return None
        */
        void post_process(std::vector<std::string> features, std::vector<Evec> &results);

    private:

        std::unique_ptr<ai2d_builder> ai2d_builder_; // ai2d构建器
        std::unique_ptr<ai2d_builder> ai2d_builder_pad_; // padding ai2d构建器
        runtime_tensor ai2d_in_tensor_;              // ai2d输入tensor
        runtime_tensor ai2d_out_tensor_;             // ai2d输出tensor
        runtime_tensor ai2d_out_tensor_pad_;         // padding 后图像输出
        uintptr_t vaddr_;                            // isp的虚拟地址
        FrameCHWSize isp_shape_;                     // isp对应的地址大小

        int topk;
        Bbox crop_box;
        float thres;
};
#endif

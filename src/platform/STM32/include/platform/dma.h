/*
 * This file is part of Betaflight.
 *
 * Betaflight is free software. You can redistribute this software
 * and/or modify this software under the terms of the GNU General
 * Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later
 * version.
 *
 * Betaflight is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "platform.h"

#if defined(STM32F4) || defined(STM32F7) || defined(STM32G4) || defined(STM32H7) || defined(APM32F4)
#define PLATFORM_TRAIT_DMA_STREAM_REQUIRED 1
#endif

#if defined(STM32F4) || defined(STM32F7)
#define DMA_ARCH_TYPE DMA_Stream_TypeDef
#elif defined(STM32H7)
// H7 has stream based DMA and channel based BDMA, but we ignore BDMA (for now).
#define DMA_ARCH_TYPE DMA_Stream_TypeDef
#else
#define DMA_ARCH_TYPE DMA_Channel_TypeDef
#endif

#if defined(STM32F4) || defined(STM32F7) || defined(STM32H7)

typedef enum {
    DMA_NONE = 0,
    DMA_FIRST_HANDLER = 1,
    DMA1_ST0_HANDLER = DMA_FIRST_HANDLER,
    DMA1_ST1_HANDLER,
    DMA1_ST2_HANDLER,
    DMA1_ST3_HANDLER,
    DMA1_ST4_HANDLER,
    DMA1_ST5_HANDLER,
    DMA1_ST6_HANDLER,
    DMA1_ST7_HANDLER,
    DMA2_ST0_HANDLER,
    DMA2_ST1_HANDLER,
    DMA2_ST2_HANDLER,
    DMA2_ST3_HANDLER,
    DMA2_ST4_HANDLER,
    DMA2_ST5_HANDLER,
    DMA2_ST6_HANDLER,
    DMA2_ST7_HANDLER,
    DMA_LAST_HANDLER = DMA2_ST7_HANDLER
} dmaIdentifier_e;

#define DMA_DEVICE_NO(x)    ((((x)-1) / 8) + 1)
#define DMA_DEVICE_INDEX(x) ((((x)-1) % 8))
#define DMA_OUTPUT_INDEX    0
#define DMA_OUTPUT_STRING   "DMA%d Stream %d:"

#define DEFINE_DMA_CHANNEL(d, s, f) { \
    .dma = d, \
    .ref = (dmaResource_t *)d ## _Stream ## s, \
    .stream = s, \
    .irqHandlerCallback = NULL, \
    .flagsShift = f, \
    .irqN = d ## _Stream ## s ## _IRQn, \
    .userParam = 0, \
    .owner.owner = 0, \
    .owner.resourceIndex = 0 \
    }

#define DEFINE_DMA_IRQ_HANDLER(d, s, i) FAST_IRQ_HANDLER void DMA ## d ## _Stream ## s ## _IRQHandler(void) {\
                                                                const uint8_t index = DMA_IDENTIFIER_TO_INDEX(i); \
                                                                dmaCallbackHandlerFuncPtr handler = dmaDescriptors[index].irqHandlerCallback; \
                                                                if (handler) \
                                                                    handler(&dmaDescriptors[index]); \
                                                            }

#define DMA_CLEAR_FLAG(d, flag) if (d->flagsShift > 31) d->dma->HIFCR = (flag << (d->flagsShift - 32)); else d->dma->LIFCR = (flag << d->flagsShift)
#define DMA_GET_FLAG_STATUS(d, flag) (d->flagsShift > 31 ? d->dma->HISR & (flag << (d->flagsShift - 32)): d->dma->LISR & (flag << d->flagsShift))

#define DMA_IT_TCIF         ((uint32_t)0x00000020)
#define DMA_IT_HTIF         ((uint32_t)0x00000010)
#define DMA_IT_TEIF         ((uint32_t)0x00000008)
#define DMA_IT_DMEIF        ((uint32_t)0x00000004)
#define DMA_IT_FEIF         ((uint32_t)0x00000001)

void dmaMuxEnable(dmaIdentifier_e identifier, uint32_t dmaMuxId);

#else

#if defined(STM32G4)

typedef enum {
    DMA_NONE = 0,
    DMA_FIRST_HANDLER = 1,
    DMA1_CH1_HANDLER = DMA_FIRST_HANDLER,
    DMA1_CH2_HANDLER,
    DMA1_CH3_HANDLER,
    DMA1_CH4_HANDLER,
    DMA1_CH5_HANDLER,
    DMA1_CH6_HANDLER,
    DMA1_CH7_HANDLER,
    DMA1_CH8_HANDLER,
    DMA2_CH1_HANDLER,
    DMA2_CH2_HANDLER,
    DMA2_CH3_HANDLER,
    DMA2_CH4_HANDLER,
    DMA2_CH5_HANDLER,
    DMA2_CH6_HANDLER,
    DMA2_CH7_HANDLER,
    DMA2_CH8_HANDLER,
    DMA_LAST_HANDLER = DMA2_CH8_HANDLER
} dmaIdentifier_e;

#define DMA_DEVICE_NO(x)    ((((x)-1) / 8) + 1)
#define DMA_DEVICE_INDEX(x) ((((x)-1) % 8) + 1)

uint32_t dmaGetChannel(const uint8_t channel);

#else // !STM32G4

typedef enum {
    DMA_NONE = 0,
    DMA_FIRST_HANDLER = 1,
    DMA1_CH1_HANDLER = DMA_FIRST_HANDLER,
    DMA1_CH2_HANDLER,
    DMA1_CH3_HANDLER,
    DMA1_CH4_HANDLER,
    DMA1_CH5_HANDLER,
    DMA1_CH6_HANDLER,
    DMA1_CH7_HANDLER,
    DMA_LAST_HANDLER = DMA1_CH7_HANDLER
} dmaIdentifier_e;

#define DMA_DEVICE_NO(x)    ((((x)-1) / 7) + 1)
#define DMA_DEVICE_INDEX(x) ((((x)-1) % 7) + 1)

#endif // STM32G4

#define DMA_OUTPUT_INDEX    0
#define DMA_OUTPUT_STRING   "DMA%d Channel %d:"

#define DEFINE_DMA_CHANNEL(d, c, f) { \
    .dma = d, \
    .ref = (dmaResource_t *)d ## _Channel ## c, \
    .irqHandlerCallback = NULL, \
    .flagsShift = f, \
    .irqN = d ## _Channel ## c ## _IRQn, \
    .userParam = 0, \
    .owner.owner = 0, \
    .owner.resourceIndex = 0 \
    }

#define DMA_HANDLER_CODE

#define DEFINE_DMA_IRQ_HANDLER(d, c, i) DMA_HANDLER_CODE void DMA ## d ## _Channel ## c ## _IRQHandler(void) {\
                                                                        const uint8_t index = DMA_IDENTIFIER_TO_INDEX(i); \
                                                                        dmaCallbackHandlerFuncPtr handler = dmaDescriptors[index].irqHandlerCallback; \
                                                                        if (handler) \
                                                                            handler(&dmaDescriptors[index]); \
                                                                    }

#define DMA_CLEAR_FLAG(d, flag) d->dma->IFCR = (flag << d->flagsShift)
#define DMA_GET_FLAG_STATUS(d, flag) (d->dma->ISR & (flag << d->flagsShift))

#define DMA_IT_TCIF         ((uint32_t)0x00000002)
#define DMA_IT_HTIF         ((uint32_t)0x00000004)
#define DMA_IT_TEIF         ((uint32_t)0x00000008)

#endif

// Macros to avoid direct register and register bit access

#if defined(STM32F4) || defined(STM32F7)
#define IS_DMA_ENABLED(reg) (((DMA_ARCH_TYPE *)(reg))->CR & DMA_SxCR_EN)
#define REG_NDTR NDTR
#elif defined(STM32H7)
// For H7, we have to differenciate DMA1/2 and BDMA for access to the control register.
// HAL library has a macro for this, but it is extremely inefficient in that it compares
// the address against all possible values.
// Here, we just compare the address against regions of memory.
#if defined(STM32H7A3xx) || defined(STM32H7A3xxQ)
// For H7A3, if it's lower than CD_AHB2PERIPH_BASE, then it's DMA1/2 and it's stream based.
// If not, it's BDMA and it's channel based.
#define IS_DMA_ENABLED(reg) \
    ((uint32_t)(reg) < CD_AHB2PERIPH_BASE) ? \
        (((DMA_Stream_TypeDef *)(reg))->CR & DMA_SxCR_EN) : \
        (((BDMA_Channel_TypeDef *)(reg))->CCR & BDMA_CCR_EN)
#else
// For H743 and H750, if it's not in D3 peripheral area, then it's DMA1/2 and it's stream based.
// If not, it's BDMA and it's channel based.
#define IS_DMA_ENABLED(reg) \
    ((uint32_t)(reg) < D3_AHB1PERIPH_BASE) ? \
        (((DMA_Stream_TypeDef *)(reg))->CR & DMA_SxCR_EN) : \
        (((BDMA_Channel_TypeDef *)(reg))->CCR & BDMA_CCR_EN)
#endif
#elif defined(STM32G4)
#define IS_DMA_ENABLED(reg) (((DMA_ARCH_TYPE *)(reg))->CCR & DMA_CCR_EN)
// Missing __HAL_DMA_SET_COUNTER in FW library V1.0.0
#define __HAL_DMA_SET_COUNTER(__HANDLE__, __COUNTER__) ((__HANDLE__)->Instance->CNDTR = (uint16_t)(__COUNTER__))
#else
#define IS_DMA_ENABLED(reg) (((DMA_ARCH_TYPE *)(reg))->CCR & DMA_CCR_EN)
#define DMAx_SetMemoryAddress(reg, address) ((DMA_ARCH_TYPE *)(reg))->CMAR = (uint32_t)&s->port.txBuffer[s->port.txBufferTail]
#endif

#if defined(USE_HAL_DRIVER)

// We actually need these LL case only

#define xLL_EX_DMA_DeInit(dmaResource) LL_EX_DMA_DeInit((DMA_ARCH_TYPE *)(dmaResource))
#define xLL_EX_DMA_Init(dmaResource, initstruct) LL_EX_DMA_Init((DMA_ARCH_TYPE *)(dmaResource), initstruct)
#define xLL_EX_DMA_DisableResource(dmaResource) LL_EX_DMA_DisableResource((DMA_ARCH_TYPE *)(dmaResource))
#define xLL_EX_DMA_EnableResource(dmaResource) LL_EX_DMA_EnableResource((DMA_ARCH_TYPE *)(dmaResource))
#define xLL_EX_DMA_GetDataLength(dmaResource) LL_EX_DMA_GetDataLength((DMA_ARCH_TYPE *)(dmaResource))
#define xLL_EX_DMA_SetDataLength(dmaResource, length) LL_EX_DMA_SetDataLength((DMA_ARCH_TYPE *)(dmaResource), length)
#define xLL_EX_DMA_EnableIT_TC(dmaResource) LL_EX_DMA_EnableIT_TC((DMA_ARCH_TYPE *)(dmaResource))

#else

#define xDMA_Init(dmaResource, initStruct) DMA_Init((DMA_ARCH_TYPE *)(dmaResource), initStruct)
#define xDMA_DeInit(dmaResource) DMA_DeInit((DMA_ARCH_TYPE *)(dmaResource))
#define xDMA_Cmd(dmaResource, newState) DMA_Cmd((DMA_ARCH_TYPE *)(dmaResource), newState)
#define xDMA_ITConfig(dmaResource, flags, newState) DMA_ITConfig((DMA_ARCH_TYPE *)(dmaResource), flags, newState)
#define xDMA_GetCurrDataCounter(dmaResource) DMA_GetCurrDataCounter((DMA_ARCH_TYPE *)(dmaResource))
#define xDMA_SetCurrDataCounter(dmaResource, count) DMA_SetCurrDataCounter((DMA_ARCH_TYPE *)(dmaResource), count)
#define xDMA_GetFlagStatus(dmaResource, flags) DMA_GetFlagStatus((DMA_ARCH_TYPE *)(dmaResource), flags)
#define xDMA_ClearFlag(dmaResource, flags) DMA_ClearFlag((DMA_ARCH_TYPE *)(dmaResource), flags)
#define xDMA_MemoryTargetConfig(dmaResource, address, target) DMA_MemoryTargetConfig((DMA_ARCH_TYPE *)(dmaResource), address, target)

#endif

#include <platform.h>
#include <gpio.h>

void gpio_toggle(Pin pin) {
}

void gpio_set(Pin pin, int value) {
	
	GPIO_TypeDef* p = GET_PORT(pin);
	uint32_t pin_index = GET_PIN_INDEX(pin);
	
	MODIFY_REG(p->ODR,1UL<<pin_index,value<<pin_index);

}

int gpio_get(Pin pin) {

	GPIO_TypeDef* p = GET_PORT(pin);
	uint32_t pin_index = GET_PIN_INDEX(pin);
	
	return READ_BIT(p->IDR,(1<<pin_index));

}



void gpio_set_mode(Pin pin, PinMode mode) {

	GPIO_TypeDef* p = GET_PORT(pin);
	uint32_t pin_index = GET_PIN_INDEX(pin);
	RCC->AHB1ENR|=1UL<<GET_PORT_INDEX(pin);
	switch(mode) {
		case Reset:
			MODIFY_REG(p->MODER, 11UL<<((pin_index)*2), 0UL<<((pin_index)*2));  
			MODIFY_REG(p->PUPDR, 11UL<<((pin_index)*2), 0UL<<((pin_index)*2));  
			break;
		case Input:
			MODIFY_REG(p->MODER, 11UL<<((pin_index)*2), 0UL<<((pin_index)*2));  
			MODIFY_REG(p->PUPDR, 11UL<<((pin_index)*2), 0UL<<((pin_index)*2));  
			break;
		case Output:
			MODIFY_REG(p->MODER, 11UL<<((pin_index)*2), 1UL<<((pin_index)*2)); 
			MODIFY_REG(p->PUPDR, 11UL<<((pin_index)*2), 0UL<<((pin_index)*2)); 
			break;
		case PullUp:
			MODIFY_REG(p->MODER, 11UL<<((pin_index)*2), 0UL<<((pin_index)*2));  
			MODIFY_REG(p->PUPDR, 11UL<<((pin_index)*2), 1UL<<((pin_index)*2)); 
			break;
		case PullDown:
			MODIFY_REG(p->MODER, 11UL<<((pin_index)*2), 0UL<<((pin_index)*2));  
			MODIFY_REG(p->PUPDR, 11UL<<((pin_index)*2), 10UL<<((pin_index)*2)); 
			break;
	}

}



// *******************************ARM University Program Copyright © ARM Ltd 2014*************************************   

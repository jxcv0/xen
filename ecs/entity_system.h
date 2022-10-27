#ifndef ENTITY_SYSTEM_H_
#define ENTITY_SYSTEM_H_

#ifndef MAX_ENTITIES
#define MAX_ENTITIES 512
#endif

#include <stdint.h>

/**
 * The entity system manages the entity ids and is used to keep track of thier
 * signature and state.
 *
 * All functions declared that are part of this system are prefixed with "es_"
 */

/**
 * @brief Initialize the entity system.
 */
void es_init(void);

/**
 * @brief Pop the next entity id off the list of free entitied affter setting
 *        it's signature to 0.
 *
 * @return An entity id that acts as a descriptor/handle used to get access to
 *         the entity or manage its lifetime.
 *         On failure this function returns -1;
 */
int es_create_entity(void);

/**
 * @brief Reset an entities signature and push the id to the back of the list of
 *        free entity ids.
 *
 * @param id The id to delete.
 */
void es_delete_entity(int id);

#endif // ENTITY_SYSTEM_H_

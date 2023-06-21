#ifndef RECOVERY_H
#define RECOVERY_H
#include <stddef.h>
#include <stdint.h>

/*/
 * Returns the data hidden in the given shadows
 * Function assumes there are at least k shadows in the array
 */
uint8_t *recover_secret(uint8_t *shadows[], uint8_t *X, size_t block_count, uint8_t k, uint8_t mod);

#endif //RECOVERY_H

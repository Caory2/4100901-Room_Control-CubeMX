#include "ring_buffer.h"

/**
    * @brief Initializes the ring buffer.
    * @param rb Pointer to the ring buffer structure.
    * @param buffer Pointer to the memory area used for the buffer.
    * @param capacity The maximum number of elements the buffer can hold.
    */



void ring_buffer_init(ring_buffer_t *rb, uint8_t *buffer, uint16_t capacity) 
{
    rb->buffer = buffer;
    rb->head = 0;
    rb->tail = 0;
    rb->capacity = capacity;
    rb->itsfull = false; // Initialize the size of the buffer to 0

}
/**
    * @brief Writes data to the ring buffer. discard the old data if is full
    * @param rb Pointer to the ring buffer structure.
    * @param data The data to be written to the buffer.
    * @return true if data was successfully written, false if the buffer is full.
    */

bool ring_buffer_write(ring_buffer_t *rb, uint8_t data)
{
    if (rb->itsfull) {
        // If the buffer is full, overwrite the oldest data
        rb->tail = (rb->tail + 1) % rb->capacity;
    }
    rb->buffer[rb->head] = data; // Write the data to the buffer
    rb->head = (rb->head + 1) % rb->capacity; // Move the head pointer
    if (rb->head == rb->tail) {
        rb->itsfull = true; // If head meets tail, the buffer is full
    } 
    return true;
}

/**
    * @brief Reads data from the ring buffer.
    * @param rb Pointer to the ring buffer structure.
    * @param data Pointer to where the read data will be stored.
    * @return true if data was successfully read, false if the buffer is empty.
    */

bool ring_buffer_read(ring_buffer_t *rb, uint8_t *data)
{
    if (rb->head == rb->tail && !rb->itsfull) {
        // If head meets tail and the buffer is not full, it is empty
        return false; 
    }
    *data = rb->buffer[rb->tail]; // Read the data from the buffer
    rb->tail = (rb->tail + 1) % rb->capacity; // Move the tail pointer
    rb->itsfull = false; // After reading, the buffer is no longer full
    return true; 
}

/**
    * @brief Returns the number of elements currently in the ring buffer.
    * @param rb Pointer to the ring buffer structure.
    * @return The number of elements in the buffer.
    */
uint16_t ring_buffer_count(ring_buffer_t *rb)
{
    if (rb->itsfull) {
        return rb->capacity; // If the buffer is full, return its capacity
    }
    if (rb->head >= rb->tail) {
        return rb->head - rb->tail; // If head is ahead of tail, return the difference
    } else {
        return rb->capacity - rb->tail + rb->head; // If head has wrapped around, calculate the count accordingly
    }

}

/**
    * @brief Checks if the ring buffer is empty.
    * @param rb Pointer to the ring buffer structure.
    * @return true if the buffer is empty, false otherwise.
    */
bool ring_buffer_is_empty(ring_buffer_t *rb)
{
    
    return (rb->head == rb->tail && !rb->itsfull); // The buffer is empty if head meets tail and it is not full
}

/**
    * @brief Checks if the ring buffer is full.
    * @param rb Pointer to the ring buffer structure.
    * @return true if the buffer is full, false otherwise.
    */
bool ring_buffer_is_full(ring_buffer_t *rb)
{
    return rb->itsfull; // The buffer is full if itsfull is true
    
}

/**
    * @brief Flushes the ring buffer, clearing all data.
    * @param rb Pointer to the ring buffer structure.
    */
void ring_buffer_flush(ring_buffer_t *rb)
{
    rb->head = 0; // Reset head pointer
    rb->tail = 0; // Reset tail pointer
    rb->itsfull = false; // Set the buffer as not full
    // No need to clear the buffer memory, as it will be overwritten on next write

}
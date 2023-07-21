# Use the official nginx image as the base image
FROM nginx:alpine

# Install ffmpeg to handle video transcoding
RUN apk update && apk add ffmpeg

# Copy the nginx configuration for RTMP streaming
COPY nginx-rtmp.conf /etc/nginx/nginx.conf
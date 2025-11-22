import hotels from "/hotels.js";

function createHotelCard(hotel) {
    const hotelCard = document.createElement("div");
    hotelCard.className = "hotel glass";

    const hotelImage = document.createElement("div");
    hotelImage.className = "hotel-image";

    const img = document.createElement("img");
    img.src = hotel.image;
    img.alt = "Hotel image";
    hotelImage.appendChild(img);
    hotelCard.appendChild(hotelImage);

    const hotelDetails = document.createElement("div");
    hotelDetails.className = "hotel-details glass";

    const nameDiv = document.createElement("div");
    nameDiv.className = "detail hotel-name";
    nameDiv.textContent = hotel.name;
    hotelDetails.appendChild(nameDiv);

    const locationDiv = document.createElement("div");
    locationDiv.className = "detail location";
    locationDiv.textContent = hotel.location;
    hotelDetails.appendChild(locationDiv);

    const priceDiv = document.createElement("div");
    priceDiv.className = "detail hotel-price";
    priceDiv.textContent = `${hotel.price.toLocaleString()}€ / 24h`;
    hotelDetails.appendChild(priceDiv);

    const amenitiesDiv = document.createElement("div");
    amenitiesDiv.className = "detail hotel-amenities";
    hotel.amenities.forEach((amenity) => {
        const span = document.createElement("span");
        span.textContent = amenity;
        amenitiesDiv.appendChild(span);
    });
    hotelDetails.appendChild(amenitiesDiv);

    const descriptionDiv = document.createElement("div");
    descriptionDiv.className = "detail hotel-description";

    const p = document.createElement("p");
    p.textContent = hotel.description;
    descriptionDiv.appendChild(p);
    hotelDetails.appendChild(descriptionDiv);
    hotelCard.appendChild(hotelDetails);

    return hotelCard;
}

document.addEventListener("DOMContentLoaded", () => {
    const hotelSection = document.getElementById("hotel");
    const params = new URLSearchParams(document.location.search);
    const hotelName = params.get("hotel");

    const hotel = hotels.find(hotel => hotel.name === hotelName);
    if (hotel) {
        hotelSection.appendChild(createHotelCard(hotel));
    } else {
        window.location.replace("/hotels/" + hotelName);
    }
});
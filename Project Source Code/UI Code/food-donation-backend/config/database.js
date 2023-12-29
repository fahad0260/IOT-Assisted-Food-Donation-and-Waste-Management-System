const mongoose = require("mongoose");
const UserModel = require("../model/userModel.js");
const DonationModel = require("../model/DonationModel.js");
const notificationModel = require("../model/notificationModel.js");

const mongoDBConnect = (() => {
  mongoose.set("strictQuery", true);
  mongoose
    .connect(process.env.MONGODB_URL)
    .then(async() => {
      console.log("connected to database");

      const userCollectionExists = await mongoose.connection.db
        .listCollections({ name: "users" }) // Change 'users' to your collection name
        .hasNext();

      if (!userCollectionExists) {
        // If the collection doesn't exist, create it
        await UserModel.createCollection();
        console.log("User collection created");
      } else {
        console.log("User collection already exists, not creating");
      }

      const notificationCollectionExists = await mongoose.connection.db
        .listCollections({ name: "notification" }) // Change 'users' to your collection name
        .hasNext();

      if (!notificationCollectionExists) {
        // If the collection doesn't exist, create it
        await notificationModel.createCollection();
        console.log("notification collection created");
      } else {
        console.log("notification collection already exists, not creating");
      }

      const donationCollectionExists = await mongoose.connection.db
        .listCollections({ name: "donation" }) // Change 'users' to your collection name
        .hasNext();

      if (!donationCollectionExists) {
        // If the collection doesn't exist, create it
        await DonationModel.createCollection();
        console.log("donation collection created");
      } else {
        console.log("donation collection already exists, not creating");
      }
    })
    .catch((err) => console.log(err));
})();

module.exports = mongoDBConnect;

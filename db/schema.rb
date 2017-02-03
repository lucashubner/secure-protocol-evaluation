# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20170123232347) do

  create_table "protocol_types", force: :cascade do |t|
    t.string   "protocolName"
    t.datetime "created_at",   null: false
    t.datetime "updated_at",   null: false
  end

  create_table "sensor_reads", force: :cascade do |t|
    t.float    "value"
    t.integer  "sensor_type_id"
    t.datetime "created_at",     null: false
    t.datetime "updated_at",     null: false
    t.index ["sensor_type_id"], name: "index_sensor_reads_on_sensor_type_id"
  end

  create_table "sensor_types", force: :cascade do |t|
    t.string   "type_name"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  create_table "statistics", force: :cascade do |t|
    t.integer  "value"
    t.integer  "protocol_type_id"
    t.datetime "created_at",       null: false
    t.datetime "updated_at",       null: false
    t.index ["protocol_type_id"], name: "index_statistics_on_protocol_type_id"
  end

end

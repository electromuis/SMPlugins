<template>
  <b-container id="app" style="padding-top: 30px; padding-bottom: 60px;">
    <b-progress v-if="loading" :value="groups.length - loadingLeft" :max="groups.length" class="mb-12" style="margin-bottom: 30px"></b-progress>

    <b-navbar fixed="bottom" style="background-color: white">
      <b-container>
        <b-form-input v-model="filter" style="display: inline-block;" :placeholder="'Search ' + items.length + ' songs'" class="w-75"></b-form-input>
        <b-button @click="reset()" class="ml-4" style="display: inline-block;">Clear</b-button>
      </b-container>
    </b-navbar>

    <b-table
     id="itemList"
     v-if="!loading"
    striped hover
    :items="itemsForList"
    :fields="fields" 
    :sort-by.sync="sortBy"
    :sort-desc.sync="sortDesc" >

      <template #cell(group)="data">
        <a href="javascript:void(0)" @click="filter = data.value">{{ data.value }}</a>
      </template>

      <template #cell(actions)="row">
        <b-button size="sm" @click="() => selectSong(row.item)">
          Select
        </b-button>
      </template>

    </b-table>

    <b-pagination
      v-if="!loading"
      v-model="currentPage"
      :total-rows="itemsFiltered.length"
      :per-page="perPage"
      align="center"
      aria-controls="itemList"
      first-number
      last-number
      limit=10
    ></b-pagination>

  </b-container>
</template>

<script>

import {client,sendRequest} from './client.js'
import _ from 'lodash'

export default {
  name: 'App',

  watch: {
    filter() {
      this.doFilter()
    },
    sortBy() {
      this.doFilter()
    },
    sortDesc() {
      this.doFilter()
    }
  },

  computed: {
    itemsForList() {
      return this.itemsFiltered.slice(
        (this.currentPage - 1) * this.perPage,
        this.currentPage * this.perPage,
      );
    }
  },
  
  data() {
    return {
      currentPage: 1,
      perPage: 10,
      items: [],
      itemsFiltered: [],
      groups: [],
      sortBy: '',
      sortDesc: false,
      fields: [
        {key: 'group', label: 'Pack', sortable: true},
        {key: 'title', label: 'Title', sortable: true},
        {key: 'artist', label: 'Artist', sortable: true},
        {key: 'charts', label: 'Charts'},
        {key: 'actions', label: 'Actions'}
      ],
      loadingLeft: 0,
      loading: true,
      filter: '',

      doFilter: function() {}
    }
  },
  methods: {
    reset() {
      this.filter = ''
      this.sortBy = ''
      this.sortDesc = false
      this.currentPage = 1
    },
    sendRequest,
    selectSong(song) {
      console.log("Select: ", song);
      this.sendRequest("SelectSong", song)
    },
    async loadSongs() {
      let me = this;
      
      me.groups = (await this.sendRequest('GetGroups', {})).groups
      me.loadingLeft = me.groups.length

      for(let g=0; g<me.groups.length; g++) {
        const group = me.groups[g]
        const songs = (await me.sendRequest("GetSongsInGroup", {group})).songs
        
        songs.forEach(song => {
          song.group = group

          if(!song.steps) {
            return
          }
        
          song.charts = song.steps
            .filter((s) => s.type == "Dance_Single")
            .map((s) => s.meter)
            .sort(function(a, b){return a-b})
            .join(', ')

          me.items.push(song)
        })

        me.loadingLeft -= 1
      }

      me.itemsFiltered = me.items
      me.loading = false
    }
  },
  created() {
    let me = this;

    client.onopen = function() {
      console.log("Open");
      me.loadSongs()
    };

    this.doFilter = _.debounce(function() {
      me.itemsFiltered = me.items.filter((i) => {
        const searchKey = [
          i.artist.toLowerCase(),
          i.title.toLowerCase(),
          i.group.toLowerCase()
        ].join(' ')

        return searchKey.includes(me.filter.toLowerCase())
      })

      me.currentPage = 1
      me.itemsFiltered.sort((a, b) => {

        if(a[me.sortBy] < b[me.sortBy]) {
          return me.sortDesc ? -1 : 1
        }

        if(a[me.sortBy] > b[me.sortBy]) {
          return me.sortDesc ? 1 : -1
        }

        return 0

      })

    }, 1000)
  }
}
</script>

<style>

</style>
